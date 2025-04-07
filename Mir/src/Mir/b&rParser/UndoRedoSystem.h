#include "classTypes.h"

template <class T>
class UndoRedoSystem {
private:
    std::vector<std::unique_ptr<T>> m_undoStack;
    std::vector<std::unique_ptr<T>> m_redoStack;
    T* m_currentTable = nullptr;
    size_t m_maxStackSize = 100; // Limit stack size to prevent memory issues
    
public:
    UndoRedoSystem() {}
    UndoRedoSystem(T* table) : m_currentTable(table) {
        // Create initial snapshot
        SaveSnapshot();
    }
    void SaveSnapshot() {
        if (!m_currentTable){
            MIR_ASSERT(false, "you forgot to intialize UndoRedoSystem");
        } 
        // Clear redo stack when a new action is recorded
        m_redoStack.clear();
        
        // Add current state to undo stack
        m_undoStack.push_back(std::unique_ptr<T>(m_currentTable->Clone()));
        
        // Maintain reasonable stack size
        if (m_undoStack.size() > m_maxStackSize) {
            m_undoStack.erase(m_undoStack.begin());
        }
    }
    
    bool CanUndo() const { return !m_undoStack.empty(); }
    bool CanRedo() const { return !m_redoStack.empty(); }
    
    void Undo() {
        if (!CanUndo()) return;
        
        // Save current state to redo stack
        m_redoStack.push_back(std::unique_ptr<T>(m_currentTable->Clone()));
        
        // Restore previous state
        *m_currentTable = *m_undoStack.back();
        m_undoStack.pop_back();
    }
    
    void Redo() {
        if (!CanRedo()) return;
        
        // Save current state to undo stack
        m_undoStack.push_back(std::unique_ptr<T>(m_currentTable->Clone()));
        
        // Restore next state
        *m_currentTable = *m_redoStack.back();
        m_redoStack.pop_back();
    }
    size_t GetUndoStackSize() const {
        return m_undoStack.size();
    }
    
    size_t GetRedoStackSize() const {
        return m_redoStack.size();
    }
};