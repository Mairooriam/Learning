#pragma once
#include <imgui.h>
#include <memory>
#include <string>

#include "b&rParser/brTypes.h"

namespace Mir {
    namespace Widgets {
        ////////////////////////////////////////
        //////// DATA RENDERER /////////////////
        ////////////////////////////////////////
        template<typename T>
        class DataRenderer {
        public:
            static void Render(const T& data);
        };

        // Declare specializations (but don't define them)
        template<>
        class DataRenderer<brTyp> {
        public:
            static void Render(const brTyp& data);
        };

        template<>
        class DataRenderer<brVarConfigCollection> {
        public:
            static void Render(const brVarConfigCollection& data);
        };
        ////////////////////////////////////////
        //////// DATA RENDERER /////////////////
        ////////////////////////////////////////

        ////////////////////////////////////////
        //////// BASE TABLE ////////////////////
        ////////////////////////////////////////
        class BaseTable {
        public:
            class Settings {
                private:
                    bool m_ShowHeaderWidth = false;
                    bool m_ShowLineNumbers = false;
                    ImGuiTableFlags m_TableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;
                    
                    friend class BaseTable; // Allow BaseTable to access private members
                    
                public:
                    // Chainable setters
                    Settings& ShowHeaderWidth(bool show) { m_ShowHeaderWidth = show; return *this; }
                    Settings& ShowLineNumbers(bool show) { m_ShowLineNumbers = show; return *this; }
                    Settings& TableFlags(ImGuiTableFlags flags) { m_TableFlags = flags; return *this; }
                    Settings& AddTableFlags(ImGuiTableFlags flags) { m_TableFlags |= flags; return *this; }
                    Settings& RemoveTableFlags(ImGuiTableFlags flags) { m_TableFlags &= ~flags; return *this; }
                    
                    // Getters
                    bool GetShowHeaderWidth() const { return m_ShowHeaderWidth; }
                    bool GetShowLineNumbers() const { return m_ShowLineNumbers; }
                    ImGuiTableFlags GetTableFlags() const { return m_TableFlags; }
                    bool HasTableFlag(ImGuiTableFlags flag) const { return (m_TableFlags & flag) != 0; }
                };
                
                // Only expose the settings object
                Settings& GetSettings() { return m_Settings; }
                const Settings& GetSettings() const { return m_Settings; }


            virtual void Render() = 0;
            
            protected:
            Settings m_Settings;
            
            void ApplyTableStyles(float indent = 0.0f) {
                ImGui::Indent(indent);
                ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 0.0f));
            }
            
            void PopTableStyles(float indent = 0.0f) {
                ImGui::Unindent(indent);
                ImGui::PopStyleVar();
            }

            BaseTable() = default;
            virtual ~BaseTable() = default;
        };
        ////////////////////////////////////////
        //////// BASE TABLE ////////////////////
        ////////////////////////////////////////

        ////////////////////////////////////////
        //////// TABLE WIDGET //////////////////
        ////////////////////////////////////////
        template<typename T>
        class TableWidget : public BaseTable {
        private:
            std::string m_Title;
            int m_ColumnCount;
            T* m_DataPtr = nullptr;  // Pointer to data (doesn't own it)
            
        public:
            TableWidget(const std::string& title = "Table", int columnCount = 3)
                : m_Title(title), m_ColumnCount(columnCount) {}
                
            ~TableWidget() override = default;
            
            // Main render method
            void Render() override;
            
            // Set the data pointer
            void SetData(T* data) { m_DataPtr = data; }
            
            // Set table properties
            //void SetTitle(const std::string& title) { m_Title = title; }
            //void SetColumnCount(int count) { m_ColumnCount = count; }
        
        protected:
            // Helper methods
            void RenderTableHeader();
            void RenderTableRow(const T& data);
            void RenderTableData(const T& data, int depth = 0);
            
            // Get the title and column count
            const std::string& GetTitle() const { return m_Title; }
            int GetColumnCount() const { return m_ColumnCount; }
        };
        ////////////////////////////////////////
        //////// TABLE WIDGET //////////////////
        ////////////////////////////////////////
    }
}