/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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

#pragma once

#include "RimEclipseCase.h"
#include "RiaDefines.h"

#include "cvfBase.h"
#include "cvfObject.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafAppEnum.h"
#include "cvfCollection.h"


class RimIdenticalGridCaseGroup;
class RimEclipseResultDefinition;
class RimEclipseStatisticsCaseCollection;
class RigMainGrid;
class RigSimWellData;


//==================================================================================================
//
// 
//
//==================================================================================================
class RimEclipseStatisticsCase : public RimEclipseCase
{
    CAF_PDM_HEADER_INIT;

public:
    RimEclipseStatisticsCase();
    virtual ~RimEclipseStatisticsCase();

    void setMainGrid(RigMainGrid* mainGrid);

    void computeStatistics();
    bool hasComputedStatistics() const;
    void clearComputedStatistics();
    void computeStatisticsAndUpdateViews();

    void updateConnectedEditorsAndReservoirViews();

    virtual bool openEclipseGridFile();
    virtual void reloadEclipseGridFile();

    RimCaseCollection* parentStatisticsCaseCollection();

    enum PercentileCalcType
    {
        NEAREST_OBSERVATION,
        HISTOGRAM_ESTIMATED,
        INTERPOLATED_OBSERVATION
    };

    caf::PdmField< bool >                                          m_calculateEditCommand;
    virtual void  updateFilePathsFromProjectPath(const QString& projectPath, const QString& oldProjectPath){}

    void populateResultSelectionAfterLoadingGrid();
 
private:
    void scheduleACTIVEGeometryRegenOnReservoirViews();

    RimIdenticalGridCaseGroup* caseGroup();

    void getSourceCases(std::vector<RimEclipseCase*>& sourceCases);

    void populateResultSelection();

    void updateSelectionListVisibilities();
    void updateSelectionSummaryLabel();
    void updatePercentileUiVisibility();


    void setWellResultsAndUpdateViews(const cvf::Collection<RigSimWellData>& sourceCaseSimWellData);

    // Pdm system overrides
    virtual void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) ;
    virtual QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions, bool * useOptionsOnly );
    virtual void fieldChangedByUi(const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue);

    virtual void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute * attribute );
    // Fields

    caf::PdmField< caf::AppEnum< RiaDefines::ResultCatType > >      m_resultType;
    caf::PdmField< caf::AppEnum< RiaDefines::PorosityModelType > >  m_porosityModel;

    caf::PdmField<QString>                                          m_selectionSummary;

    caf::PdmField<std::vector<QString> >                            m_selectedDynamicProperties;
    caf::PdmField<std::vector<QString> >                            m_selectedStaticProperties;
    caf::PdmField<std::vector<QString> >                            m_selectedGeneratedProperties;
    caf::PdmField<std::vector<QString> >                            m_selectedInputProperties;

    caf::PdmField<std::vector<QString> >                            m_selectedFractureDynamicProperties;
    caf::PdmField<std::vector<QString> >                            m_selectedFractureStaticProperties;
    caf::PdmField<std::vector<QString> >                            m_selectedFractureGeneratedProperties;
    caf::PdmField<std::vector<QString> >                            m_selectedFractureInputProperties;

 
    caf::PdmField< bool >                                           m_calculatePercentiles;
    caf::PdmField< caf::AppEnum< PercentileCalcType > >             m_percentileCalculationType;
    caf::PdmField<double >                                          m_lowPercentile;
    caf::PdmField<double >                                          m_midPercentile;
    caf::PdmField<double >                                          m_highPercentile;

    caf::PdmField<QString>                                          m_wellDataSourceCase;

    caf::PdmField< bool >                                           m_useZeroAsInactiveCellValue;

    bool                                                            m_populateSelectionAfterLoadingGrid;
};