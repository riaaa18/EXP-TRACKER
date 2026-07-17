#include "mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox>
#include <QToolButton>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QSplitter>
#include <QDate>
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_selectedCategoryColor(QStringLiteral("#3B82F6")) // default placeholder color
{
    setupUi();
    applyStyleSheet();
}

// ============================================================================
// setupUi()
// Top-level assembly of the window: builds each of the four visual regions
// and arranges them using nested layouts.
//
// Overall structure:
//
//   QMainWindow
//     └── centralWidget (QWidget)
//           └── mainVLayout (QVBoxLayout)
//                 ├── Summary Panel (top)
//                 └── contentSplitter (QSplitter, horizontal)
//                       ├── Left Panel  (Transaction form + Category mgmt)
//                       └── Right Panel (Filter bar + Transactions table)
// ============================================================================
void MainWindow::setupUi()
{
    setWindowTitle(QStringLiteral("Expense Tracker"));
    resize(1200, 750);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main vertical layout: summary panel stacked on top of the main content area.
    QVBoxLayout *mainVLayout = new QVBoxLayout(centralWidget);
    mainVLayout->setContentsMargins(16, 16, 16, 16);
    mainVLayout->setSpacing(16);

    // ---- 1. Top Panel / Summary Overview ----
    mainVLayout->addWidget(createSummaryPanel());

    // ---- Splitter holding the Left (form) and Right (data view) panels ----
    QSplitter *contentSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    contentSplitter->setChildrenCollapsible(false);
    contentSplitter->addWidget(createLeftPanel());
    contentSplitter->addWidget(createRightPanel());

    // Give the right (table) panel more stretch space than the left (form) panel.
    contentSplitter->setStretchFactor(0, 0);
    contentSplitter->setStretchFactor(1, 1);
    contentSplitter->setSizes(QList<int>() << 340 << 860);

    mainVLayout->addWidget(contentSplitter, /*stretch=*/1);
}

// ============================================================================
// createSummaryPanel()
// Builds the top overview bar with three "stat cards": Balance, Income,
// Expenses. Each card is a small QFrame containing a title label and a
// large value label, arranged horizontally.
// ============================================================================
QWidget* MainWindow::createSummaryPanel()
{
    QFrame *summaryFrame = new QFrame;
    summaryFrame->setObjectName(QStringLiteral("summaryFrame"));

    QHBoxLayout *summaryLayout = new QHBoxLayout(summaryFrame);
    summaryLayout->setContentsMargins(20, 16, 20, 16);
    summaryLayout->setSpacing(24);

    // --- helper lambda to build one "stat card" (title + big value) ---
    auto buildStatCard = [&](const QString &title, QLabel *&valueLabelOut,
                              const QString &objectNameForValue) -> QWidget* {
        QFrame *card = new QFrame;
        card->setObjectName(QStringLiteral("statCard"));

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(16, 12, 16, 12);
        cardLayout->setSpacing(4);

        QLabel *titleLabel = new QLabel(title);
        titleLabel->setObjectName(QStringLiteral("statTitleLabel"));

        valueLabelOut = new QLabel(QStringLiteral("$0.00"));
        valueLabelOut->setObjectName(objectNameForValue);

        cardLayout->addWidget(titleLabel);
        cardLayout->addWidget(valueLabelOut);
        return card;
    };

    summaryLayout->addWidget(buildStatCard(QStringLiteral("Total Balance"),
                                            m_totalBalanceValueLabel,
                                            QStringLiteral("balanceValueLabel")));
    summaryLayout->addWidget(buildStatCard(QStringLiteral("Total Income"),
                                            m_totalIncomeValueLabel,
                                            QStringLiteral("incomeValueLabel")));
    summaryLayout->addWidget(buildStatCard(QStringLiteral("Total Expenses"),
                                            m_totalExpensesValueLabel,
                                            QStringLiteral("expenseValueLabel")));

    return summaryFrame;
}

// ============================================================================
// createLeftPanel()
// Builds the left-hand column, which stacks the Transaction Form group box
// on top of the Category Management group box.
// ============================================================================
QWidget* MainWindow::createLeftPanel()
{
    QWidget *leftPanel = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(16);

    leftLayout->addWidget(createTransactionForm());
    leftLayout->addWidget(createCategoryManagement());
    leftLayout->addStretch(1); // push everything up, leaving empty space at bottom

    return leftPanel;
}

// ============================================================================
// createTransactionForm()
// Section A of the Left Panel: a QFormLayout with Amount, Category, Date,
// Type (Income/Expense radio buttons), Notes, and an "Add Transaction" button.
// ============================================================================
QGroupBox* MainWindow::createTransactionForm()
{
    QGroupBox *groupBox = new QGroupBox(QStringLiteral("New Transaction"));

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setContentsMargins(12, 16, 12, 12);
    formLayout->setSpacing(10);
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    // --- Amount ---
    m_amountSpinBox = new QDoubleSpinBox;
    m_amountSpinBox->setRange(0.0, 1000000.0);
    m_amountSpinBox->setDecimals(2);
    m_amountSpinBox->setPrefix(QStringLiteral("$ "));
    m_amountSpinBox->setSingleStep(1.0);
    formLayout->addRow(QStringLiteral("Amount:"), m_amountSpinBox);

    // --- Category ---
    m_categoryComboBox = new QComboBox;
    m_categoryComboBox->addItems({QStringLiteral("Food"),
                                   QStringLiteral("Utilities"),
                                   QStringLiteral("Rent"),
                                   QStringLiteral("Entertainment"),
                                   QStringLiteral("Salary")});
    formLayout->addRow(QStringLiteral("Category:"), m_categoryComboBox);

    // --- Date (defaults to today) ---
    m_dateEdit = new QDateEdit(QDate::currentDate());
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat(QStringLiteral("yyyy-MM-dd"));
    formLayout->addRow(QStringLiteral("Date:"), m_dateEdit);

    // --- Type: Income vs Expense (QRadioButton pair inside a QButtonGroup) ---
    QWidget *typeContainer = new QWidget;
    QHBoxLayout *typeLayout = new QHBoxLayout(typeContainer);
    typeLayout->setContentsMargins(0, 0, 0, 0);
    typeLayout->setSpacing(16);

    m_incomeRadioButton = new QRadioButton(QStringLiteral("Income"));
    m_expenseRadioButton = new QRadioButton(QStringLiteral("Expense"));
    m_expenseRadioButton->setChecked(true); // sensible default

    QButtonGroup *typeButtonGroup = new QButtonGroup(this);
    typeButtonGroup->addButton(m_incomeRadioButton);
    typeButtonGroup->addButton(m_expenseRadioButton);

    typeLayout->addWidget(m_incomeRadioButton);
    typeLayout->addWidget(m_expenseRadioButton);
    typeLayout->addStretch(1);

    formLayout->addRow(QStringLiteral("Type:"), typeContainer);

    // --- Notes / Description ---
    m_notesLineEdit = new QLineEdit;
    m_notesLineEdit->setPlaceholderText(QStringLiteral("Optional description..."));
    formLayout->addRow(QStringLiteral("Notes:"), m_notesLineEdit);

    // --- Add Transaction button (spans the row, right-aligned) ---
    m_addTransactionButton = new QPushButton(QStringLiteral("Add Transaction"));
    m_addTransactionButton->setObjectName(QStringLiteral("primaryButton"));
    connect(m_addTransactionButton, &QPushButton::clicked,
            this, &MainWindow::onAddTransactionClicked);

    QHBoxLayout *buttonRow = new QHBoxLayout;
    buttonRow->addStretch(1);
    buttonRow->addWidget(m_addTransactionButton);
    formLayout->addRow(buttonRow);

    groupBox->setLayout(formLayout);
    return groupBox;
}

// ============================================================================
// createCategoryManagement()
// Section B of the Left Panel: a small inline form for adding a custom
// category name plus a color preview/picker placeholder, and a button that
// (in the real app) would append the new category to m_categoryComboBox.
// ============================================================================
QGroupBox* MainWindow::createCategoryManagement()
{
    QGroupBox *groupBox = new QGroupBox(QStringLiteral("Manage Categories"));

    QVBoxLayout *outerLayout = new QVBoxLayout;
    outerLayout->setContentsMargins(12, 16, 12, 12);
    outerLayout->setSpacing(10);

    // Row: [ new category name field ][ color swatch button ]
    QHBoxLayout *inputRow = new QHBoxLayout;
    inputRow->setSpacing(8);

    m_newCategoryLineEdit = new QLineEdit;
    m_newCategoryLineEdit->setPlaceholderText(QStringLiteral("New category name..."));

    // Simple color preview/picker placeholder: a small flat button showing
    // the current color; clicking it (in a full implementation) would open
    // a QColorDialog. Kept minimal here since this is UI-only.
    m_categoryColorButton = new QToolButton;
    m_categoryColorButton->setObjectName(QStringLiteral("colorSwatchButton"));
    m_categoryColorButton->setFixedSize(32, 32);
    m_categoryColorButton->setToolTip(QStringLiteral("Pick a color for this category"));
    m_categoryColorButton->setStyleSheet(
        QStringLiteral("background-color: %1; border-radius: 4px;").arg(m_selectedCategoryColor));
    connect(m_categoryColorButton, &QToolButton::clicked,
            this, &MainWindow::onPickCategoryColorClicked);

    inputRow->addWidget(m_newCategoryLineEdit, /*stretch=*/1);
    inputRow->addWidget(m_categoryColorButton);

    // "Add Category" button, right-aligned beneath the input row.
    m_addCategoryButton = new QPushButton(QStringLiteral("Add Category"));
    m_addCategoryButton->setObjectName(QStringLiteral("secondaryButton"));
    connect(m_addCategoryButton, &QPushButton::clicked,
            this, &MainWindow::onAddCategoryClicked);

    QHBoxLayout *buttonRow = new QHBoxLayout;
    buttonRow->addStretch(1);
    buttonRow->addWidget(m_addCategoryButton);

    outerLayout->addLayout(inputRow);
    outerLayout->addLayout(buttonRow);

    groupBox->setLayout(outerLayout);
    return groupBox;
}

// ============================================================================
// createRightPanel()
// Builds the right/main panel: a filter bar stacked above the transactions
// table, inside a vertical layout.
// ============================================================================
QWidget* MainWindow::createRightPanel()
{
    QWidget *rightPanel = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(12);

    // --- Filter bar ---
    rightLayout->addWidget(createFilterBar());

    // --- Transactions table ---
    m_transactionsTable = new QTableWidget(0, 5); // 0 rows to start, 5 columns
    m_transactionsTable->setObjectName(QStringLiteral("transactionsTable"));

    QStringList headers;
    headers << QStringLiteral("Date")
            << QStringLiteral("Type")
            << QStringLiteral("Category")
            << QStringLiteral("Description")
            << QStringLiteral("Amount");
    m_transactionsTable->setHorizontalHeaderLabels(headers);

    m_transactionsTable->horizontalHeader()->setStretchLastSection(true);
    m_transactionsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_transactionsTable->verticalHeader()->setVisible(false);
    m_transactionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_transactionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_transactionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_transactionsTable->setAlternatingRowColors(true);
    m_transactionsTable->setShowGrid(false);

    // ---- Placeholder sample rows purely for visual demonstration ----
    // (No data persistence/backend logic — these are static UI examples.)
    struct SampleRow { QString date, type, category, description, amount; };
    const QList<SampleRow> sampleRows = {
        {QStringLiteral("2026-07-01"), QStringLiteral("Income"),  QStringLiteral("Salary"),        QStringLiteral("Monthly paycheck"),   QStringLiteral("$4,200.00")},
        {QStringLiteral("2026-07-03"), QStringLiteral("Expense"), QStringLiteral("Rent"),           QStringLiteral("Apartment rent"),     QStringLiteral("-$1,200.00")},
        {QStringLiteral("2026-07-05"), QStringLiteral("Expense"), QStringLiteral("Food"),           QStringLiteral("Groceries"),          QStringLiteral("-$85.40")},
        {QStringLiteral("2026-07-06"), QStringLiteral("Expense"), QStringLiteral("Entertainment"),  QStringLiteral("Movie night"),        QStringLiteral("-$32.00")},
    };

    m_transactionsTable->setRowCount(sampleRows.size());
    for (int row = 0; row < sampleRows.size(); ++row) {
        const SampleRow &r = sampleRows.at(row);
        m_transactionsTable->setItem(row, 0, new QTableWidgetItem(r.date));
        m_transactionsTable->setItem(row, 1, new QTableWidgetItem(r.type));
        m_transactionsTable->setItem(row, 2, new QTableWidgetItem(r.category));
        m_transactionsTable->setItem(row, 3, new QTableWidgetItem(r.description));
        m_transactionsTable->setItem(row, 4, new QTableWidgetItem(r.amount));
    }

    rightLayout->addWidget(m_transactionsTable, /*stretch=*/1);

    return rightPanel;
}

// ============================================================================
// createFilterBar()
// A thin horizontal bar above the table: a category filter dropdown and a
// text search field.
// ============================================================================
QWidget* MainWindow::createFilterBar()
{
    QFrame *filterFrame = new QFrame;
    filterFrame->setObjectName(QStringLiteral("filterFrame"));

    QHBoxLayout *filterLayout = new QHBoxLayout(filterFrame);
    filterLayout->setContentsMargins(12, 8, 12, 8);
    filterLayout->setSpacing(10);

    QLabel *filterLabel = new QLabel(QStringLiteral("Filter:"));

    m_filterCategoryComboBox = new QComboBox;
    m_filterCategoryComboBox->addItem(QStringLiteral("All Categories"));
    m_filterCategoryComboBox->addItems({QStringLiteral("Food"),
                                         QStringLiteral("Utilities"),
                                         QStringLiteral("Rent"),
                                         QStringLiteral("Entertainment"),
                                         QStringLiteral("Salary")});
    connect(m_filterCategoryComboBox, &QComboBox::currentTextChanged,
            this, &MainWindow::onFilterChanged);

    m_searchLineEdit = new QLineEdit;
    m_searchLineEdit->setPlaceholderText(QStringLiteral("Search description..."));
    connect(m_searchLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::onFilterChanged);

    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(m_filterCategoryComboBox);
    filterLayout->addWidget(m_searchLineEdit, /*stretch=*/1);

    return filterFrame;
}

// ============================================================================
// applyStyleSheet()
// A single, centralized QSS block giving the whole window a clean, modern
// "light mode" appearance. Object names set above (e.g. "statCard",
// "primaryButton") are used as CSS-like selectors here.
// ============================================================================
void MainWindow::applyStyleSheet()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #F5F6FA;
            color: #1F2430;
            font-family: "Segoe UI", "Helvetica Neue", Arial, sans-serif;
            font-size: 13px;
        }

        QMainWindow {
            background-color: #F5F6FA;
        }

        /* ---- Summary panel ---- */
        #summaryFrame {
            background-color: #FFFFFF;
            border: 1px solid #E3E6ED;
            border-radius: 10px;
        }

        #statCard {
            background-color: #FAFBFD;
            border: 1px solid #ECEEF3;
            border-radius: 8px;
        }

        #statTitleLabel {
            color: #6B7280;
            font-size: 12px;
            font-weight: 600;
            text-transform: uppercase;
        }

        #balanceValueLabel {
            font-size: 22px;
            font-weight: 700;
            color: #1F2430;
        }

        #incomeValueLabel {
            font-size: 22px;
            font-weight: 700;
            color: #16A34A; /* green */
        }

        #expenseValueLabel {
            font-size: 22px;
            font-weight: 700;
            color: #DC2626; /* red */
        }

        /* ---- Group boxes (Transaction Form / Category Management) ---- */
        QGroupBox {
            background-color: #FFFFFF;
            border: 1px solid #E3E6ED;
            border-radius: 10px;
            margin-top: 10px;
            font-weight: 600;
            padding-top: 6px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 4px;
            color: #374151;
        }

        /* ---- Inputs ---- */
        QLineEdit, QComboBox, QDoubleSpinBox, QDateEdit {
            background-color: #FFFFFF;
            border: 1px solid #D6DAE3;
            border-radius: 6px;
            padding: 5px 8px;
            min-height: 22px;
        }

        QLineEdit:focus, QComboBox:focus, QDoubleSpinBox:focus, QDateEdit:focus {
            border: 1px solid #3B82F6;
        }

        /* ---- Buttons ---- */
        QPushButton {
            border-radius: 6px;
            padding: 7px 16px;
            font-weight: 600;
            border: 1px solid transparent;
        }

        #primaryButton {
            background-color: #3B82F6;
            color: #FFFFFF;
        }
        #primaryButton:hover {
            background-color: #2563EB;
        }
        #primaryButton:pressed {
            background-color: #1D4ED8;
        }

        #secondaryButton {
            background-color: #EEF2FF;
            color: #3B4CCA;
            border: 1px solid #C7D2FE;
        }
        #secondaryButton:hover {
            background-color: #E0E7FF;
        }

        #colorSwatchButton {
            border: 1px solid #D6DAE3;
        }

        /* ---- Filter bar ---- */
        #filterFrame {
            background-color: #FFFFFF;
            border: 1px solid #E3E6ED;
            border-radius: 8px;
        }

        /* ---- Table ---- */
        #transactionsTable {
            background-color: #FFFFFF;
            border: 1px solid #E3E6ED;
            border-radius: 8px;
            gridline-color: #EEF0F4;
            alternate-background-color: #F9FAFC;
        }

        QHeaderView::section {
            background-color: #F3F4F8;
            color: #374151;
            padding: 6px;
            border: none;
            border-bottom: 1px solid #E3E6ED;
            font-weight: 600;
        }

        QTableWidget::item {
            padding: 4px;
        }

        QTableWidget::item:selected {
            background-color: #DBEAFE;
            color: #1F2430;
        }

        /* ---- Splitter handle ---- */
        QSplitter::handle {
            background-color: transparent;
            width: 8px;
        }
    )");
}

// ============================================================================
// Placeholder slots.
// These intentionally contain NO business logic / persistence. They exist so
// the UI is fully wired up and ready for a backend to be plugged in later.
// ============================================================================
void MainWindow::onAddTransactionClicked()
{
    // TODO: Hook up to backend/data layer.
    // For now, just a placeholder acknowledgement so the button is functional.
    QMessageBox::information(this, QStringLiteral("Add Transaction"),
                              QStringLiteral("Transaction form submitted (placeholder — no backend wired up)."));
}

void MainWindow::onAddCategoryClicked()
{
    // TODO: Validate input and persist the new category via backend.
    const QString newCategory = m_newCategoryLineEdit->text().trimmed();
    if (newCategory.isEmpty()) {
        return;
    }

    // Dynamically expand both the transaction-form category dropdown and
    // the filter dropdown, purely as a UI convenience.
    if (m_categoryComboBox->findText(newCategory) == -1) {
        m_categoryComboBox->addItem(newCategory);
    }
    if (m_filterCategoryComboBox->findText(newCategory) == -1) {
        m_filterCategoryComboBox->addItem(newCategory);
    }

    m_newCategoryLineEdit->clear();
}

void MainWindow::onPickCategoryColorClicked()
{
    // Simple color picker placeholder using the standard Qt color dialog.
    const QColor chosen = QColorDialog::getColor(QColor(m_selectedCategoryColor), this,
                                                  QStringLiteral("Choose Category Color"));
    if (chosen.isValid()) {
        m_selectedCategoryColor = chosen.name();
        m_categoryColorButton->setStyleSheet(
            QStringLiteral("background-color: %1; border-radius: 4px;").arg(m_selectedCategoryColor));
    }
}

void MainWindow::onFilterChanged()
{
    // TODO: Apply category + text filtering to the transactions table/model.
    // Left empty intentionally — this is a UI-only scaffold.
}
