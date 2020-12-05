// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Blueprint/UserWidget.h"

#include "BattreyMan.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ABattreyMan : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ABattreyMan();

    //variables
    //---------------------------------------------------------
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    USpringArmComponent* CameraBoom; // Camera Boom/Arm

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    UCameraComponent* FollowCamera; // Camera

    bool bDead; // Is dead?

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Power; // Power Variable

    UPROPERTY(EditAnywhere)
    float Power_Treshold; // Power Max

    // Public Functions Declaration
    //---------------------------------------------------------

    // Function to Move Forward/backward
    void MoveForward(float Axis);

    // Function to Move Right/Left
    void MoveRight(float Axis);

    // Function to Restart the game
    void RestartGame();

    // Function If Player Collied with pickup, add to the "Power" Variable
    // Destroy the object
    UFUNCTION()
    void OnBeginOverlap(class UPrimitiveComponent* HitComp,
                        class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Function for UI: "Power" amount bar
    UPROPERTY(EditAnywhere, Category = "UI HUD")
    TSubclassOf<UUserWidget> Player_Power_Widget_class;

    UUserWidget* Player_Power_Widget;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

// TODO: 1:00:23
