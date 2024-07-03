// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "iostream"
#include "Navigation/PathFollowingComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIBehaviourTreeGameGameMode.h"

//"D:/Progetti Unreal/TagGame/Content/BB_BlackboardData.uasset"

AEnemyAIController::AEnemyAIController()
{
	ConstructorHelpers::FObjectFinder<UBlackboardData>myBlackboard(TEXT("/Script/AIModule.BlackboardData'/Game/BB_Blackboard.BB_Blackboard'"));
	if (myBlackboard.Succeeded())
	{
		BlackboardAsset = myBlackboard.Object;
		UE_LOG(LogTemp, Warning, TEXT("Loaded Succesfully"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load the blackboard"));
	}
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (UseBlackboard(BlackboardAsset, BlackboardComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Blackboard initialized successfully"));
		
		BlackboardComponent->SetValueAsObject(TEXT("BestBall"), nullptr);
		BlackboardComponent->SetValueAsObject(TEXT("NearestBall"), nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to initialize blackboard"));
	}
	
	InitializeStates();
}

void AEnemyAIController::InitializeStates()
{
    GoToPlayer = MakeShared<FAivState>(
        [](AAIController* AIController) {
            AIController->MoveToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), 100.0f);
        },
        nullptr,
        [this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
            EPathFollowingStatus::Type State = AIController->GetMoveStatus();

            if (State == EPathFollowingStatus::Moving)
            {
                return nullptr;
            }

            UObject* BestBallObj = BlackboardComponent->GetValueAsObject(TEXT("BestBall"));
            ABall* BestBall = Cast<ABall>(BestBallObj);

            if (BestBall)
            {
                BestBall->AttachToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
                BestBall->SetActorRelativeLocation(FVector(0, 0, 0));
                BlackboardComponent->SetValueAsObject(TEXT("BestBall"), nullptr);
            }
            return SearchForBall;
        }
    );

    SearchForBall = MakeShared<FAivState>(
        [this](AAIController* AIController) {
            AGameModeBase* GameMode = AIController->GetWorld()->GetAuthGameMode();
            AAIBehaviourTreeGameGameMode* AIGameMode = Cast<AAIBehaviourTreeGameGameMode>(GameMode);
            const TArray<ABall*>& BallsList = AIGameMode->GetBalls();

            UObject* NearestBallObj = BlackboardComponent->GetValueAsObject(TEXT("NearestBall"));
            ABall* NearestBall = Cast<ABall>(NearestBallObj);

            for (int32 i = 0; i < BallsList.Num(); i++)
            {
                if (!BallsList[i]->GetAttachParentActor() &&
                    (!NearestBall ||
                        FVector::Distance(AIController->GetPawn()->GetActorLocation(), BallsList[i]->GetActorLocation()) <
                        FVector::Distance(AIController->GetPawn()->GetActorLocation(), NearestBall->GetActorLocation())))
                {
                    NearestBall = BallsList[i];
                }
            }

            BlackboardComponent->SetValueAsObject(TEXT("BestBall"), NearestBall);
        },
        nullptr,
        [this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
            UObject* BestBallObj = BlackboardComponent->GetValueAsObject(TEXT("BestBall"));
            if (BestBallObj)
            {
                return GoToBall;
            }
            else 
            {
                return SearchForBall;
            }
        }
    );

    GoToBall = MakeShared<FAivState>(
        [this](AAIController* AIController) {
            UObject* BestBallObj = BlackboardComponent->GetValueAsObject(TEXT("BestBall"));
            ABall* BestBall = Cast<ABall>(BestBallObj);
            AIController->MoveToActor(BestBall, 100.0f);
        },
        nullptr,
        [this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
            EPathFollowingStatus::Type State = AIController->GetMoveStatus();

            if (State == EPathFollowingStatus::Moving)
            {
                return nullptr;
            }
            return GrabBall;
        }
    );

    GrabBall = MakeShared<FAivState>(
        [this](AAIController* AIController)
        {
            UObject* BestBallObj = BlackboardComponent->GetValueAsObject(TEXT("BestBall"));
            ABall* BestBall = Cast<ABall>(BestBallObj);
            if (BestBall && BestBall->GetAttachParentActor())
            {
                BlackboardComponent->SetValueAsObject(TEXT("BestBall"), nullptr);
            }
        },
        nullptr,
        [this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
            UObject* BestBallObj = BlackboardComponent->GetValueAsObject(TEXT("BestBall"));
            ABall* BestBall = Cast<ABall>(BestBallObj);

            if (!BestBall)
            {
                return SearchForBall;
            }

            BestBall->AttachToActor(AIController->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
            BestBall->SetActorRelativeLocation(FVector(0, 0, 0));

            return GoToPlayer;
        }
    );

    CurrentState = SearchForBall;
    CurrentState->CallEnter(this);
}

void AEnemyAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (CurrentState)
    {
        CurrentState = CurrentState->CallTick(this, DeltaTime);
    }
}
