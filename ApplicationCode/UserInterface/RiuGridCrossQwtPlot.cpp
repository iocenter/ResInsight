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
#include "RiuGridCrossQwtPlot.h"

#include "RiuCvfOverlayItemWidget.h"
#include "RiuWidgetDragger.h"

#include "RimGridCrossPlot.h"
#include "RimGridCrossPlotCurveSet.h"
#include "RimRegularLegendConfig.h"

#include "cafTitledOverlayFrame.h"

#include <QResizeEvent>

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RiuGridCrossQwtPlot::RiuGridCrossQwtPlot(RimViewWindow* ownerViewWindow, QWidget* parent /*= nullptr*/)
    : RiuQwtPlot(ownerViewWindow, parent)
{    
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuGridCrossQwtPlot::addOrUpdateCurveSetLegend(RimGridCrossPlotCurveSet* curveSet)
{
    RiuCvfOverlayItemWidget* overlayWidget = nullptr;

    auto it = m_legendWidgets.find(curveSet);
    if (it == m_legendWidgets.end() || it->second == nullptr)
    {
        overlayWidget = new RiuCvfOverlayItemWidget(this);

        new RiuWidgetDragger(overlayWidget);

        m_legendWidgets[curveSet] = overlayWidget;
    }
    else
    {
        overlayWidget = it->second;
    }

    if (overlayWidget)
    {
        caf::TitledOverlayFrame* overlayItem = curveSet->legendConfig()->titledOverlayFrame();
        resizeOverlayItemToFitPlot(overlayItem);
        overlayWidget->updateFromOverlayItem(overlayItem);

    }
    this->updateLegendLayout();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuGridCrossQwtPlot::removeCurveSetLegend(RimGridCrossPlotCurveSet* curveSetToShowLegendFor)
{
    auto it = m_legendWidgets.find(curveSetToShowLegendFor);
    if (it != m_legendWidgets.end())
    {
        if (it->second != nullptr) it->second->deleteLater();

        m_legendWidgets.erase(it);
    }

    this->updateLegendLayout();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuGridCrossQwtPlot::updateLegendSizesToMatchPlot()
{
    RimGridCrossPlot* crossPlot = dynamic_cast<RimGridCrossPlot*>(ownerPlotDefinition());
    if (!crossPlot) return;

    bool anyLegendResized = false;

    for (RimGridCrossPlotCurveSet* curveSet : crossPlot->curveSets())
    {
        if (!curveSet->isChecked() || !curveSet->legendConfig()->showLegend()) continue;

        auto pairIt = m_legendWidgets.find(curveSet);
        if (pairIt != m_legendWidgets.end())
        {
            RiuCvfOverlayItemWidget* overlayWidget = pairIt->second;
            if (overlayWidget->isVisible())
            {
                caf::TitledOverlayFrame* overlayItem = curveSet->legendConfig()->titledOverlayFrame();
                if (resizeOverlayItemToFitPlot(overlayItem))
                {
                    anyLegendResized = true;
                    overlayWidget->updateFromOverlayItem(overlayItem);
                }
            }
        }
    }
    if (anyLegendResized)
    {
        updateLegendLayout();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuGridCrossQwtPlot::updateLayout()
{
    QwtPlot::updateLayout();
    updateLegendLayout();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuGridCrossQwtPlot::updateLegendLayout()
{
    const int spacing      = 5;
    int       startMarginX = this->canvas()->pos().x() + spacing;
    int       startMarginY = this->canvas()->pos().y() + spacing;

    int xpos           = startMarginX;
    int ypos           = startMarginY;
    int maxColumnWidth = 0;

    RimGridCrossPlot* crossPlot = dynamic_cast<RimGridCrossPlot*>(ownerPlotDefinition());

    if (!crossPlot) return;

    std::set<QString> legendTypes;

    for (RimGridCrossPlotCurveSet* curveSet : crossPlot->curveSets())
    {
        if (!curveSet->isChecked() || !curveSet->legendConfig()->showLegend()) continue;

        auto pairIt = m_legendWidgets.find(curveSet);
        if (pairIt != m_legendWidgets.end())
        {
            RiuCvfOverlayItemWidget* overlayWidget = pairIt->second;
            // Show only one copy of each legend type
            if (!legendTypes.count(curveSet->groupParameter()))
            {
                if (ypos + overlayWidget->height() + spacing > this->canvas()->height())
                {
                    xpos += spacing + maxColumnWidth;
                    ypos = startMarginY;
                    maxColumnWidth = 0;
                }

                overlayWidget->show();
                overlayWidget->move(xpos, ypos);

                ypos += pairIt->second->height() + spacing;
                maxColumnWidth = std::max(maxColumnWidth, pairIt->second->width());
                legendTypes.insert(curveSet->groupParameter());
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RiuGridCrossQwtPlot::resizeEvent(QResizeEvent* e)
{
    QwtPlot::resizeEvent(e);
    updateLegendSizesToMatchPlot();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RiuGridCrossQwtPlot::resizeOverlayItemToFitPlot(caf::TitledOverlayFrame* overlayItem)
{
    QSize       plotSize   = this->canvas()->contentsRect().size();
    cvf::Vec2ui legendSize = overlayItem->preferredSize();

    bool sizeAltered = false;

    if (plotSize.width() > 0 && (double) legendSize.x() > 0.9 * plotSize.width())
    {
        legendSize.x() = (plotSize.width() * 9) / 10;
        sizeAltered = true;
    }
    if (plotSize.height() > 0 && (double) legendSize.y() > 0.9 * plotSize.height())
    {
        legendSize.y() = (plotSize.height() * 9) / 10;
        sizeAltered = true;
    }
    overlayItem->setRenderSize(legendSize);
    return sizeAltered;
}