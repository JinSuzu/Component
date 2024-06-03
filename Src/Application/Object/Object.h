#pragma once
class Object
{
public:
	virtual bool GetActive() const { return m_bActive; }
	virtual void SetActive(bool _active) { m_bActive = _active; }

	virtual void Destroy() { m_bDestroy = true; }
	virtual bool GetDestroy() const { return m_bDestroy; }

	const int m_instanceID = (int)(this);
protected:
	bool m_bDestroy = false;
	bool m_bActive = true;
};