#pragma once
#include <Component.h>
#include <string>

struct PlayerStats
{
	int Lives = 3;
	int Score = 0;
};

enum class Key;
class Subject;
class TextComponent;
class QBertPlayer final : public Component
{
public:
	QBertPlayer() = default;
	virtual ~QBertPlayer();

	void Initialize() override;
	void Render() const override;
	void Update() override;

	const PlayerStats& GetStats() const { return m_Stats; };
	void AddScore(int amount);
	void OnDeath();
	void OnRespawn();
	void OnScored();

	void SetKillButton(Key button);
	void SetName(const std::string& name);
	void SetRandomPointsButton(Key button);

	void SetSubject(Subject* pSubject, bool isOverwrite = false);
	Subject* GetSubject() const { return m_pSubject; };

	void SetLivesDisplay(TextComponent* pTc) { m_pLifesDisplay = pTc; };
	void SetScoreDisplay(TextComponent* pTc) { m_pScoreDisplay = pTc; };

private:
	TextComponent* m_pLifesDisplay;
	TextComponent* m_pScoreDisplay;
	Subject* m_pSubject;
	bool m_IsKilled = false, m_HasScored = false;
	PlayerStats m_Stats;
	std::string m_Name = "QBert";
	Key m_KillButton;
	Key m_RandomPointsButton;
};