// Fill out your copyright notice in the Description page of Project Settings.


#include "BattreyMan.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABattreyMan::ABattreyMan()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set Capsule Components Size
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);


    // Set up player Control
    //---------------------------------------------------------
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;
    //---------------------------------------------------------

    // set up Camera Bom/Arm
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Set up Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Starts the player Alive
    bDead = false;
    // Power
    Power = 100.0f;
}

// Called when the game starts or when spawned
void ABattreyMan::BeginPlay()
{
    Super::BeginPlay();

    // To see if player Collied with pickup
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABattreyMan::OnBeginOverlap);

    // setup the UI/Power bar
    if (Player_Power_Widget_class != nullptr)
    {
        Player_Power_Widget = CreateWidget(GetWorld(), Player_Power_Widget_class);
        Player_Power_Widget->AddToViewport();
    }
    
}

// Called every frame
void ABattreyMan::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Power -= DeltaTime * Power_Treshold;

    if(Power <= 0)
    {
        // die
        bDead = true;
        // fall down
        GetMesh()->SetSimulatePhysics(true);

        // Start a timer to re-start the game
        FTimerHandle GameRestartTimerHandle;
        GetWorldTimerManager().SetTimer(
            GameRestartTimerHandle, this, &ABattreyMan::RestartGame, 3.0f, false);
    }
}

// Called to bind functionality to input
void ABattreyMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Binds the moues X axis Left/Right
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    // Binds the mouse Y axis up/down
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    // Binds the Space Key to Jump
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    // Released to stop Jumping
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    // Binds W/S keys to Forward/Backward
    PlayerInputComponent->BindAxis("MoveForward", this, &ABattreyMan::MoveForward);
    // Binds A/D kes to Left/Right
    PlayerInputComponent->BindAxis("MoveRight", this, &ABattreyMan::MoveRight);
}

// Move Forward/Baclward
void ABattreyMan::MoveForward(float Axis)
{
    if (!bDead)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Axis);
    }
}

// Move Left/Right
void ABattreyMan::MoveRight(float Axis)
{
    if (!bDead)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Axis);
    }
}

void ABattreyMan::RestartGame()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

// To see if player Collied with pickup
void ABattreyMan::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Checks to see if The item has a Tag "Recharge"
    if (OtherActor->ActorHasTag("Recharge"))
    {
        /*
         * If object Collied with has a Tag "Recharge"
         * Add to the Players power variable
         * Makes sher if at max Power don't add to it
         * Destroy the Pickup object
         */
        Power += 10.0f;

        if (Power > 100.0f)
        {
            Power = 100.0f;
        }

        OtherActor->Destroy();
    }
}
