#pragma once
#include "b&rParser/brTypes.h"  // Include necessary headers for brVarConfigCollection
#include "b&rParser/parser.h"   // Include for brparser
#include "imgui.h"
// Function declarations
namespace Mir {
    void RenderIODataTable(const brVarConfigCollection& VarConfigCollection, bool displayCellSize);
    void RenderVariableTable(const brVarConfigNode& varConfig, size_t tableIndex, bool displayCellSize);
    void RenderTableRows(const brVarConfigNode& varConfig, size_t tableIndex);
    void RenderImGuiEditor(brparser& brparser);
    
}