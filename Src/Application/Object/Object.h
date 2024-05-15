#pragma once
class Object
{
public:
	bool GetActive() const { return m_bActive; }
	void SetActive(bool _active) { m_bActive = _active; }

	void Destroy() { m_bDestroy = true; }
	bool GetDestroy() const { return m_bDestroy; }

protected:
	bool m_bDestroy = false;
	bool m_bActive = true;
};