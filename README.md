# Quick Charts

A QtQuick module providing high-performance charts.

## Introduction

The Quick Charts module provides a set of charts that can be used from QtQuick
applications. They are intended to be used for both simple display of data as
well as continuous display of high-volume data (often referred to as plotters).
The charts use a system called distance fields for their accelerated rendering,
which provides ways of using the GPU for rendering 2D shapes without loss of
quality.

### Usage Example

The following piece of code will render a simple line chart containing three
lines:

\include{lineno} examples/snippets/minimal.qml

## Concepts

There are three main concepts to consider when using this module: charts, data
sources and decorations.

### Charts

These are the main items of the module. They process data and render it in a
certain way. Currently there are three main types of charts: pie charts, line
charts and bar charts. All charts inherit the [Chart] class, which provides the
most basic chart interface. On top of that is the [XYChart] base class, which
provides an interface for charts that are based on an X/Y grid.

* [PieChart](\ref org::kde::quickcharts::PieChart)
* [LineChart](\ref org::kde::quickcharts::LineChart)
* [BarChart](\ref org::kde::quickcharts::BarChart)

[Chart]: \ref org::kde::quickcharts::Chart
[XYChart]: \ref org::kde::quickcharts::XYChart

### Data Sources

Data sources are objects that provide data to charts. These objects act as
adapters to other objects, like Qt's models. All data source objects inherit
[ChartDataSource], which represents the basic data source interface.

* [SingleValueSource](\ref org::kde::quickcharts::SingleValueSource)
* [ArraySource](\ref org::kde::quickcharts::ArraySource)
* [ModelSource](\ref org::kde::quickcharts::ModelSource)
* [ValueHistorySource](\ref org::kde::quickcharts::ValueHistorySource)
* [ModelHistorySource](\ref org::kde::quickcharts::ModelHistorySource)
* [ColorGradientSource](\ref org::kde::quickcharts::ColorGradientSource)
* [ChartAxisSource](\ref org::kde::quickcharts::ChartAxisSource)

[ChartDataSource]: \ref org::kde::quickcharts::ChartDataSource

### Decorations

Decorations are items that provide extra information about a chart. These are
things like a legend or axis labels. They usually work with multiple types of
charts, though some may be limited to X/Y charts.

* [AxisLabels](\ref org::kde::quickcharts::AxisLabels)
* [GridLines](\ref org::kde::quickcharts::GridLines)
* [LegendModel](\ref org::kde::quickcharts::LegendModel)

## Controls

A submodule is provided that contains pre-made controls that are provided as a
convenience layer. These controls may be more restrictive in their data sources
or how they display things. They are composed of Charts items along with some
QtQuick Controls items. Some of the controls may use style-specific theming.

* [Legend](\ref org::kde::quickcharts::controls::Legend)
* [LegendDelegate](\ref org::kde::quickcharts::controls::LegendDelegate)
* [LineChartControl](\ref org::kde::quickcharts::controls::LineChartControl)
* [PieChartControl](\ref org::kde::quickcharts::controls::PieChartControl)
