#ifndef __FAPBVP_GTK_PROV_H
#define __FAPBVP_GTK_PROV_H

#include <fapbase.h>
#include <fapfact.h>
#include <gtk/gtk.h>

// Provider
class CagProvider: public CAE_ProviderBase
{
    public:
	CagProvider(GtkWidget* aWnd);
	virtual ~CagProvider();
	// From MAE_Provider
	virtual CAE_StateBase* CreateStateL(const char *aTypeUid, const char* aInstName, CAE_Object* aMan) const { return NULL;};
	virtual CAE_EBase* CreateObjectL(TUint32 aTypeUid) const { return NULL;};
	virtual CAE_EBase* CreateObjectL(const char *aName) const { return NULL;};
	virtual const TTransInfo* GetTransf(const char *aName) const { return NULL;};
	virtual void RegisterState(const TStateInfo *aInfo) {};
	virtual void RegisterStates(const TStateInfo **aInfos) {};
	virtual void RegisterTransf(const TTransInfo *aTrans) {};
	virtual void RegisterTransfs(const TTransInfo **aNames) {};
	virtual const CAE_Formatter* GetFormatter(int aUid) const { return NULL;};
	virtual void RegisterFormatter(CAE_Formatter *aForm) {};
	virtual CAE_ChromoBase* CreateChromo() const { return NULL;};
	virtual CAE_TranExBase* CreateTranEx(MCAE_LogRec* aLogger) const { return NULL;};
	virtual MAE_Opv* CreateViewProxy();
    private:
	GtkWidget* iWnd;
};


#endif 
