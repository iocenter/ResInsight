/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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

#include "RimWellMeasurement.h"

#include "RimProject.h"
#include "RimWellMeasurementCollection.h"
#include "RimWellPath.h"

CAF_PDM_SOURCE_INIT( RimWellMeasurement, "WellMeasurement" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimWellMeasurement::RimWellMeasurement()
{
    CAF_PDM_InitObject( "RimWellMeasurement", "", "", "" );

    CAF_PDM_InitFieldNoDefault( &m_wellName, "WellName", "Well Name", "", "", "" );
    CAF_PDM_InitField( &m_MD, "Depth", -1.0, "MD", "", "", "" );
    CAF_PDM_InitFieldNoDefault( &m_date, "Date", "Date", "", "", "" );
    CAF_PDM_InitField( &m_value, "Value", 0.0, "Value", "", "", "" );
    CAF_PDM_InitFieldNoDefault( &m_kind, "Kind", "Kind", "", "", "" );
    CAF_PDM_InitFieldNoDefault( &m_quality, "Quality", "Quality", "", "", "" );
    CAF_PDM_InitFieldNoDefault( &m_remark, "Remark", "Remark", "", "", "" );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimWellMeasurement::~RimWellMeasurement() {}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimWellMeasurement::wellName() const
{
    return m_wellName();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::setWellName( const QString& wellName )
{
    m_wellName = wellName;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimWellMeasurement::MD() const
{
    return m_MD();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::setMD( double md )
{
    m_MD = md;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QDate RimWellMeasurement::date() const
{
    return m_date();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::setDate( const QDate& date )
{
    m_date = date;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
double RimWellMeasurement::value() const
{
    return m_value();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::setValue( double value )
{
    m_value = value;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimWellMeasurement::kind() const
{
    return m_kind();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::setKind( const QString& kind )
{
    m_kind = kind;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
int RimWellMeasurement::quality() const
{
    return m_quality();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::setQuality( int quality )
{
    m_quality = quality;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimWellMeasurement::remark() const
{
    return m_remark();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::setRemark( const QString& remark )
{
    m_remark = remark;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::fieldChangedByUi( const caf::PdmFieldHandle* changedField,
                                           const QVariant&            oldValue,
                                           const QVariant&            newValue )
{
    RimProject* proj;
    this->firstAncestorOrThisOfTypeAsserted( proj );
    proj->scheduleCreateDisplayModelAndRedrawAllViews();

    RimWellMeasurementCollection* wellMeasurementCollection;
    this->firstAncestorOrThisOfTypeAsserted( wellMeasurementCollection );
    wellMeasurementCollection->updateAllReferringTracks();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimWellMeasurement::defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) {}
