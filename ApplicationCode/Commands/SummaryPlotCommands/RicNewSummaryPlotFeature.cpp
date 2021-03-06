/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RicNewSummaryPlotFeature.h"

#include "RiaApplication.h"
#include "RiaPreferences.h"
#include "RiaSummaryTools.h"

#include "RicEditSummaryPlotFeature.h"
#include "RicSummaryCurveCreator.h"
#include "RicSummaryCurveCreatorDialog.h"

#include "RimEnsembleCurveFilter.h"
#include "RimEnsembleCurveFilterCollection.h"
#include "RimProject.h"
#include "RimRegularLegendConfig.h"
#include "RimSummaryCase.h"
#include "RimSummaryCaseCollection.h"
#include "RimSummaryCaseMainCollection.h"
#include "RimSummaryCurveFilter.h"
#include "RimSummaryPlot.h"
#include "RimSummaryPlotCollection.h"

#include "RiuPlotMainWindow.h"

#include "cafSelectionManagerTools.h"
#include "cvfAssert.h"

#include <QAction>

CAF_CMD_SOURCE_INIT( RicNewSummaryPlotFeature, "RicNewSummaryPlotFeature" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicNewSummaryPlotFeature::isCommandEnabled()
{
    RimSummaryPlotCollection* sumPlotColl = nullptr;

    caf::PdmObject* selObj = dynamic_cast<caf::PdmObject*>( caf::SelectionManager::instance()->selectedItem() );
    if ( selObj )
    {
        sumPlotColl = RiaSummaryTools::parentSummaryPlotCollection( selObj );
    }

    auto ensembleFilter     = dynamic_cast<RimEnsembleCurveFilter*>( selObj );
    auto ensembleFilterColl = dynamic_cast<RimEnsembleCurveFilterCollection*>( selObj );
    auto legendConfig       = dynamic_cast<RimRegularLegendConfig*>( selObj );

    if ( ensembleFilter || ensembleFilterColl || legendConfig ) return false;
    if ( sumPlotColl ) return true;

    // Multiple case selections
    std::vector<caf::PdmUiItem*> selectedItems = caf::selectedObjectsByTypeStrict<caf::PdmUiItem*>();

    for ( auto item : selectedItems )
    {
        if ( !dynamic_cast<RimSummaryCase*>( item ) && !dynamic_cast<RimSummaryCaseCollection*>( item ) ) return false;
    }
    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicNewSummaryPlotFeature::onActionTriggered( bool isChecked )
{
    RimProject* project = RiaApplication::instance()->project();
    CVF_ASSERT( project );

    std::vector<RimSummaryCase*>           selectedCases  = caf::selectedObjectsByType<RimSummaryCase*>();
    std::vector<RimSummaryCaseCollection*> selectedGroups = caf::selectedObjectsByType<RimSummaryCaseCollection*>();

    std::vector<caf::PdmObject*> sourcesToSelect( selectedCases.begin(), selectedCases.end() );

    if ( sourcesToSelect.empty() && selectedGroups.empty() )
    {
        const auto allSingleCases = project->firstSummaryCaseMainCollection()->topLevelSummaryCases();
        const auto allGroups      = project->summaryGroups();
        std::vector<RimSummaryCaseCollection*> allEnsembles;
        for ( const auto group : allGroups )
            if ( group->isEnsemble() ) allEnsembles.push_back( group );

        if ( !allSingleCases.empty() )
        {
            sourcesToSelect.push_back( allSingleCases.front() );
        }
        else if ( !allEnsembles.empty() )
        {
            sourcesToSelect.push_back( allEnsembles.front() );
        }
    }

    // Append grouped cases
    for ( auto group : selectedGroups )
    {
        if ( group->isEnsemble() )
        {
            sourcesToSelect.push_back( group );
        }
        else
        {
            auto groupCases = group->allSummaryCases();
            sourcesToSelect.insert( sourcesToSelect.end(), groupCases.begin(), groupCases.end() );
        }
    }

    auto dialog = RicEditSummaryPlotFeature::curveCreatorDialog();

    if ( !dialog->isVisible() )
    {
        dialog->show();
    }
    else
    {
        dialog->raise();
    }

    dialog->updateFromDefaultCases( sourcesToSelect );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicNewSummaryPlotFeature::setupActionLook( QAction* actionToSetup )
{
    actionToSetup->setText( "Open Summary Plot Editor" );
    actionToSetup->setIcon( QIcon( ":/SummaryPlotLight16x16.png" ) );
}

//==================================================================================================
///
//==================================================================================================

#include "RicNewSummaryCurveFeature.h"
#include "RicSummaryPlotFeatureImpl.h"
#include "RimMainPlotCollection.h"
#include "RiuPlotMainWindowTools.h"

CAF_CMD_SOURCE_INIT( RicNewDefaultSummaryPlotFeature, "RicNewDefaultSummaryPlotFeature" );

std::pair<RimSummaryPlotCollection*, std::vector<RimSummaryCase*>> extractSumPlotCollectionOrSelectedSumCasesFromSelection()
{
    std::vector<RimSummaryCase*> selectedSumCases;
    RimSummaryPlotCollection*    sumPlotColl = nullptr;

    std::vector<caf::PdmUiItem*> selectedItems;
    caf::SelectionManager::instance()->selectedItems( selectedItems );

    if ( selectedItems.size() )
    {
        caf::PdmObject* selObj = dynamic_cast<caf::PdmObject*>( selectedItems[0] );
        sumPlotColl            = RiaSummaryTools::parentSummaryPlotCollection( selObj );
    }

    if ( !sumPlotColl )
    {
        caf::SelectionManager::instance()->objectsByTypeStrict( &selectedSumCases );
    }

    return std::make_pair( sumPlotColl, selectedSumCases );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicNewDefaultSummaryPlotFeature::isCommandEnabled()
{
    auto sumPlotSumCasesPair = extractSumPlotCollectionOrSelectedSumCasesFromSelection();

    return sumPlotSumCasesPair.first || sumPlotSumCasesPair.second.size();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicNewDefaultSummaryPlotFeature::onActionTriggered( bool isChecked )
{
    auto sumPlotSumCasesPair = extractSumPlotCollectionOrSelectedSumCasesFromSelection();

    std::vector<RimSummaryCase*> summaryCasesToUse;

    if ( sumPlotSumCasesPair.first )
    {
        auto sumCaseVector = RiaApplication::instance()->project()->allSummaryCases();

        if ( sumCaseVector.size() )
        {
            summaryCasesToUse.push_back( sumCaseVector[0] );
        }
    }
    else if ( sumPlotSumCasesPair.second.size() )
    {
        summaryCasesToUse = sumPlotSumCasesPair.second;
    }

    if ( summaryCasesToUse.size() )
    {
        RimSummaryPlotCollection* sumPlotColl =
            RiaApplication::instance()->project()->mainPlotCollection()->summaryPlotCollection();
        RimSummaryPlot* newPlot = sumPlotColl->createSummaryPlotWithAutoTitle();

        for ( RimSummaryCase* sumCase : summaryCasesToUse )
        {
            RicSummaryPlotFeatureImpl::addDefaultCurvesToPlot( newPlot, sumCase );
        }

        newPlot->applyDefaultCurveAppearances();
        newPlot->loadDataAndUpdate();

        sumPlotColl->updateConnectedEditors();

        RiuPlotMainWindowTools::setExpanded( newPlot );
        RiuPlotMainWindowTools::selectAsCurrentItem( newPlot );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicNewDefaultSummaryPlotFeature::setupActionLook( QAction* actionToSetup )
{
    actionToSetup->setText( "New Summary Plot" );
    actionToSetup->setIcon( QIcon( ":/SummaryPlotLight16x16.png" ) );
}
