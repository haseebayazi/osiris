/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*

*  FileName: CComboLabels.cpp
*  Author:   Douglas Hoffman
*
*/

#include "CComboLabels.h"
#include "CPanelPlotToolbar.h"
#include "CMenuLabels.h"
#include <wx/arrstr.h>
#include "wxIDS.h"
#include "Platform.h"

//  static constants for default choices, must be in sync
// default selection for combo box
const wxString CComboLabels::g_sAlleles("Alleles");
const wxString CComboLabels::g_sBPS("BPS");
const wxString CComboLabels::g_sRFU("RFU");
const wxString CComboLabels::g_sTime("Time");
const wxString CComboLabels::g_sPeakArea("Peak Area");
const wxString CComboLabels::g_sILS_BPS("ILS Ref. BPS");
// default selection for menu
const int CMenuLabels::g_nDefault(IDmenuDisplayAlleles);

//*******************************************************************
//
//                                           CComboLabels
//


void CComboLabels::GetChoices(
  wxArrayString *pas,  // array of display strings
  nwxArrayInt *panID,  // array of menu IDs
  bool bPlot,
  bool bAllowPeakArea,
  int nBase)
{
  //
  // this should move to CMenuLabels or its own class
  //
  // the order of pas must be consistent with
  // enum LABEL_PLOT_TYPE and LABEL_CELL_TYPE 
  // in LABEL_TYPES.h

  size_t nAlloc = 5;
  if(pas != NULL)
  {
    pas->Alloc(nAlloc);
    if(bPlot)
    {
      pas->Add("None");
    }
    pas->Add(g_sAlleles);
    pas->Add(g_sBPS);
    pas->Add(g_sRFU);
    pas->Add(g_sTime);
    pas->Add(g_sILS_BPS);
    if(bAllowPeakArea || !bPlot)
    {
      pas->Add(g_sPeakArea);
    }
  }
  if(panID != NULL)
  {
    panID->Alloc(nAlloc);
    if(bPlot)
    {
      panID->Add(IDmenuDisplayNone + nBase);
    }
    panID->Add(IDmenuDisplayAlleles + nBase);
    panID->Add(IDmenuDisplayBPS + nBase);
    panID->Add(IDmenuDisplayRFU + nBase);
    panID->Add(IDmenuDisplayTime + nBase);
    panID->Add(IDmenuDisplayILS_BPS + nBase);
    if(bAllowPeakArea || !bPlot)
    {
      panID->Add(IDmenuDisplayPeakArea + nBase);
    }
  }
}


CComboLabels::CComboLabels(wxWindow *parent, wxWindowID id) 
    : wxComboBox()
{
  wxArrayString asChoices;
  GetChoices(&asChoices,NULL,false, true, 0);

  if(!Create(parent,id,g_sAlleles,wxDefaultPosition,wxDefaultSize,
    asChoices,wxCB_DROPDOWN | wxCB_READONLY))
  {
    ; // error
  }
  else
  {
    SetToolTip("Select information to display in table");
  }
}
CComboLabels::~CComboLabels() {;}
void CComboLabels::CheckSetSelection(wxComboBox *p,int n)
{
  if( (n >= 0) && 
      (n < (int)p->GetCount()) && 
      (n != p->GetSelection())
    )
  {
    // check if already selected to 
    // prevent unnecessary events
    // -- according to wx documentation
    //  this should not trigger an event 
    //  but still checking anyway
    p->SetSelection(n);
  }
}
void CComboLabels::SelectByMenu(CMenuLabels *pMenu)
{
  int n = pMenu->GetCheckedOffset();
  CheckSetSelection(n);
}
bool CComboLabels::PeakAreaLabelEnabled()
{
  int n = (int)GetCount();
  bool bRtn = (n == (int)LABEL_PLOT_TYPE_COUNT);
  return bRtn;
}
void CComboLabels::EnablePeakAreaLabel(bool b)
{
  bool bCurrent = PeakAreaLabelEnabled();
  if(b != bCurrent)
  {
    if(b)
    {
      Append(g_sPeakArea);
    }
    else
    {
      Delete(GetCount() - 1);
    }
  }
}
CComboLabels *CComboLabels::Clone()
{
  CComboLabels *pNew = new CComboLabels(GetParent(),GetId());
  pNew->Select(GetCurrentSelection());
  return pNew;
}
//*******************************************************************
//
//                                           CComboLabelsName
//
void CComboLabelsName::GetChoices(
  wxArrayString *pas, nwxArrayInt *panID, int nOffset)
{
  pas->Empty();
  pas->Alloc(2);
  // names must be in numeric order by IDs
  // because the selection offset is used to determine value
  pas->Add("File name");
  pas->Add("Sample name");
  if(panID != NULL)
  {
    panID->Empty();
    panID->Alloc(2);
    panID->Add(IDmenuDisplayNameFile + nOffset);
    panID->Add(IDmenuDisplayNameSample + nOffset);
  }
}
CComboLabelsName::~CComboLabelsName() {;}
CComboLabelsName::CComboLabelsName(wxWindow *parent, wxWindowID id) 
    : wxComboBox()
{
  wxArrayString asChoices;
  GetChoices(&asChoices);

  if(!Create(parent,id,asChoices.Item(0),
    wxDefaultPosition,wxDefaultSize,
    asChoices,wxCB_DROPDOWN | wxCB_READONLY))
  {
    ; // error
  }
  else 
  {
    SetToolTip(
      "Select file names or sample names\n"
      "for the row labels in the table below");
  }
}
void CComboLabelsName::SelectByMenu(CMenuLabels *pMenu)
{
  int n = pMenu->GetCheckedOffset(true);
  CheckSetSelection(n);
}


