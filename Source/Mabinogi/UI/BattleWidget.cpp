// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleWidget.h"

void UBattleWidget::DrawLine(const FVector2D& _Start, const FVector2D& _End)
{
	LineStartPoint = _Start;
	LineEndPoint = _End;
	LineColor = FColor::Red;
}
