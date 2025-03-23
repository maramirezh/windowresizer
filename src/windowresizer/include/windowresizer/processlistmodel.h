#pragma once

#include <QAbstractTableModel>
#include "windowresizerutils/processutils.h"
#include "windowresizerutils/processdata.h"

namespace winresizer {
    /**
     * @class ProcessListModel
     * @brief A model to represent a list of processes in a table view.
     */
    class ProcessListModel : public QAbstractTableModel
    {
        Q_OBJECT

    public:
        /**
        * @enum ProcessListColumn
        * @brief Columns of the process list table.
        */
        enum ProcessListColumn : uint8_t {
            ProcessName, ///< Column for process name.
            ProcessId, ///< Column for process ID.
			Max, ///< Maximum number of columns.
        };

		/**
		 * @brief Constructs a ProcessListModel object.
		 * @param parent The parent object.
		 */
        explicit ProcessListModel(std::unique_ptr<OSInterface> osInterface,
            QObject* parent = nullptr);

        /**
        * @brief Provides header data for the table.
        * @param section The section of the header.
        * @param orientation The orientation of the header.
        * @param role The role for which the data is requested.
        * @return The header data.
        */
        QVariant headerData(int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;

        /**
         * @brief Returns the number of rows in the model.
         * @param parent The parent index.
         * @return The number of rows.
         */
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;

        /**
         * @brief Returns the number of columns in the model.
         * @param parent The parent index.
         * @return The number of columns.
         */
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

		/**
		 * @brief Returns the data for the given index and role.
		 * @param index The index of the data.
		 * @param role The role of the data.
		 * @return The data for the given index and role.
		 */
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    public slots:

        /**
         * @brief Resets the model and updates the process list.
         * This method clears the current process list and fetches the updated list of processes.
         * It then emits the necessary signals to update the view.
         */
        void resetModel();
    private:
        ProcessUtils processUtils; ///< Process utility object
		std::vector<ProcessData> processList; ///< List of process data.
    };
}
