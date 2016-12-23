#include "MyMissionManager.h"
#include "MyControl.h"
#include <algorithm>

MyMissionManager::MyMissionManager()
{
	currentTrans = 0;
	maxCurrency = 0;
}


MyMissionManager::~MyMissionManager()
{
}

bool MyMissionManager::NewMission(MyMission * m)
{
	if (m == 0) {
		return false;
	}
	missionList.push_back(m);
	m->Start();
	if (!IsFull()) {
		m->Resume();
		currentTrans++;
	}
	return true;
}

void MyMissionManager::ProgressChange(MyMission *m, float progress, unsigned int speed)
{
	std::vector<MyMission*>::iterator result = find(missionList.begin(), missionList.end(), m);
	if (result == missionList.end()) {
		return;
	}
	MyControl::Instance()->GetProgress(this, result - missionList.begin(), progress, speed);
}

void MyMissionManager::CompleteChange(MyMission *m)
{
	std::vector<MyMission*>::iterator result = find(missionList.begin(), missionList.end(), m);
	if (result == missionList.end()) {
		return;
	}
	if (result + 1 != missionList.end()) {
		(*(result + 1))->Resume();
	}
	currentTrans--;
	MyControl::Instance()->Complete(this, result - missionList.begin());
}

bool MyMissionManager::Resume(unsigned int pos)
{
	if (pos >= missionList.size()) {
		return false;
	}
	return missionList.at(pos)->Resume();
}

bool MyMissionManager::ResumeAll()
{
	for (unsigned int i = 0; i < missionList.size(); ++i) {
		if (missionList.at(i)->IsComplete()) {
			missionList.at(i)->Start();
			missionList.at(i)->Resume();
		}
	}
	return true;
}

bool MyMissionManager::Suspend(unsigned int pos)
{
	if (pos >= missionList.size()) {
		return false;
	}
	missionList.at(pos)->Suspend();
	return true;
}

bool MyMissionManager::Suspend(std::vector<int> &poss)
{
	for (unsigned int i = 0; i < poss.size(); ++i) {
		if (poss.at(i) < 0 || (unsigned int)poss.at(i) > missionList.size()) {
			return false;
		}
	}
	
	for (unsigned int i = 0; i < poss.size(); ++i) {
		missionList.at(poss[i])->Suspend();
	}
	return true;
}

bool MyMissionManager::SuspendAll() {
	for (unsigned int i = 0; i < missionList.size(); ++i) {
		missionList.at(i)->Suspend();
	}
	return true;
}

bool MyMissionManager::Cancel(unsigned int pos)
{
	if (pos >= missionList.size()){
		return false;
	}

	MyMission* temp = missionList.at(pos);
	temp->Close();
	temp->Cancel();
	temp->Release();
	Cleaner::Delete<MyMission*>(&temp);
	missionList.erase(missionList.begin() + pos);

	return true;
}

bool MyMissionManager::CancelAll()
{
	MyMission* temp = 0;
	for (unsigned int i = 0; i < missionList.size(); ++i) {
		temp = missionList.at(i);
		temp->Close();
		temp->Cancel();
		temp->Release();
		Cleaner::Delete<MyMission*>(&temp);
		missionList.erase(missionList.begin() + i);
	}
	return false;
}

void MyMissionManager::CloseAllThread()
{
	for (unsigned int i = 0; i < missionList.size(); ++i) {
		if (!missionList[i]->IsFinish()) {
			missionList[i]->Close();
		}
	}
}

void MyMissionManager::Release()
{
	for (unsigned int i = 0; i < missionList.size(); ++i) {
		missionList.at(i)->Release();
		Cleaner::Delete<MyMission*>(&(missionList.at(i)));
	}
}

