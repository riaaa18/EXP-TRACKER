#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward declarations to keep the header lightweight and speed up compilation.
class QLabel;
class QLineEdit;
class QComboBox;
class QDoubleSpinBox;
class QDateEdit;
class QPushButton;
class QRadioButton;
class QTableWidget;
class QGroupBox;
class QToolButton;

/**
 * @brief The MainWindow class represents the main application window
 *        for the Expense Tracking desktop application.
 *
 * This class is UI-ONLY. It builds the entire interface programmatically
 * (no .ui files). All backend/data-persistence logic is intentionally
 * left out and represented by placeholder/empty slots.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private:
    // ---------- UI construction helpers ----------
    // Each method builds one visual region of the window and returns
    // the top-level widget/layout for that region. Splitting the
    // construction this way keeps the constructor readable.
    void setupUi();
    QWidget*   createSummaryPanel();      // Top panel: Balance / Income / Expenses
    QWidget*   createLeftPanel();         // Left panel: Transaction form + Category management
    QGroupBox* createTransactionForm();   // Left panel - section A
    QGroupBox* createCategoryManagement();// Left panel - section B
    QWidget*   createRightPanel();        // Right panel: filter bar + transactions table
    QWidget*   createFilterBar();         // Filter bar above the table

    void applyStyleSheet();               // Central place for the QSS "look and feel"

    // ---------- Summary (Top Panel) widgets ----------
    QLabel *m_totalBalanceValueLabel;
    QLabel *m_totalIncomeValueLabel;
    QLabel *m_totalExpensesValueLabel;

    // ---------- Transaction Form (Left Panel - A) widgets ----------
    QDoubleSpinBox *m_amountSpinBox;
    QComboBox      *m_categoryComboBox;
    QDateEdit      *m_dateEdit;
    QRadioButton   *m_incomeRadioButton;
    QRadioButton   *m_expenseRadioButton;
    QLineEdit      *m_notesLineEdit;
    QPushButton    *m_addTransactionButton;

    // ---------- Category Management (Left Panel - B) widgets ----------
    QLineEdit   *m_newCategoryLineEdit;
    QToolButton *m_categoryColorButton;   // Simple color-preview/picker placeholder
    QPushButton *m_addCategoryButton;
    QString      m_selectedCategoryColor; // Holds the currently chosen color (placeholder state)

    // ---------- Filter Bar (Right Panel) widgets ----------
    QComboBox *m_filterCategoryComboBox;
    QLineEdit *m_searchLineEdit;

    // ---------- Transactions Table (Right Panel) widgets ----------
    QTableWidget *m_transactionsTable;

private slots:
    // Placeholder slots only - NO business logic / persistence implemented here.
    void onAddTransactionClicked();
    void onAddCategoryClicked();
    void onPickCategoryColorClicked();
    void onFilterChanged();
};

#endif // MAINWINDOW_H
