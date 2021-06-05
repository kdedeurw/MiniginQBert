#include "pch.h"
#include "QBertRedBall.h"
#include "QBertEvents.h"
#include "QBertLevel.h"

QBertRedBall::QBertRedBall()
{
}

QBertRedBall::~QBertRedBall()
{
}

void QBertRedBall::Initialize()
{
}

void QBertRedBall::Render() const
{
}

void QBertRedBall::Update()
{
}

void QBertRedBall::Kill(bool hasFallen)
{
	hasFallen;
	GetLevel()->RemoveEnemy(this);
}