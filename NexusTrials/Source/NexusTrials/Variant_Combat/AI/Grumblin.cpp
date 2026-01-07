#include "Grumblin.h"
#include "NexusTrialsCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AGrumblin::AGrumblin()
{
    PrimaryActorTick.bCanEverTick = true;

    UBoxComponent* Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
    RootComponent = Box;
    Box->SetBoxExtent(FVector(50, 50, 50));
    Box->SetCollisionProfileName(TEXT("Pawn"));
    Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Box->SetGenerateOverlapEvents(true);

    Box->OnComponentBeginOverlap.AddDynamic(this, &AGrumblin::OnOverlapBegin);
}

void AGrumblin::BeginPlay()
{
    Super::BeginPlay();
}

void AGrumblin::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGrumblin::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ANexusTrialsCharacter* Player = Cast<ANexusTrialsCharacter>(OtherActor))
    {
        if (Player->GetPowerUpState() == EPowerUpState::AegisCharm)
        {
            Destroy(); // Aegis Charm kills Grumblin
        }
        else
        {
            // Normal bump/kill player logic
            Player->Die();
        }
    }
}