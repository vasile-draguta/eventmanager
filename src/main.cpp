#include "gui/GUI.h"
#include "repository/FileRepository.h"
#include "ui/UI.h"

 int startWithConsole() {
    std::unique_ptr<Repository> adminRepo = std::make_unique<FileRepository>();
    std::unique_ptr<Repository> userRepo;
    std::string userRepoType;
    std::cout << "Enter the type of the user repository (CSV/HTML): ";
    std::cin >> userRepoType;

    if (userRepoType == "CSV") {
        userRepo = std::make_unique<CSVRepository>();
    }
    else if (userRepoType == "HTML") {
        userRepo = std::make_unique<HTMLRepository>();
    }
    Service service(*adminRepo, *userRepo);
    UI ui(service);
    ui.runMenu();
    return 0;
}

std::string getUserRepositoryType() {
    std::string repoType;
    QDialog dialog;
    dialog.setWindowTitle("Select Repository Type");
    dialog.setWindowIcon(QIcon("../icon.jpg"));

    auto *csvButton = new QRadioButton("CSV");
    auto *htmlButton = new QRadioButton("HTML");

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    auto *layout = new QVBoxLayout();
    layout->addWidget(csvButton);
    layout->addWidget(htmlButton);
    layout->addWidget(buttonBox);
    dialog.setLayout(layout);

    if (dialog.exec() == QDialog::Accepted && (csvButton->isChecked() || htmlButton->isChecked())) {
        if (csvButton->isChecked()) {
            repoType = "CSV";
        } else if (htmlButton->isChecked()) {
            repoType = "HTML";
        }
    }
    else{
        dialog.close();
        QMessageBox::information(nullptr, "Information", "Nothing selected! Exiting...");
    }

    return repoType;
}

int startWithGUI(int argc, char *argv[]) {
    auto app = QApplication(argc, argv);
    std::unique_ptr<Repository> adminRepo = std::make_unique<FileRepository>();
    std::unique_ptr<Repository> userRepo;
    std::string userRepoType = getUserRepositoryType();

    if (userRepoType == "CSV") {
        userRepo = std::make_unique<CSVRepository>();
    }
    else if (userRepoType == "HTML") {
        userRepo = std::make_unique<HTMLRepository>();
    }
    else{
        return 0;
    }

    Service service(*adminRepo, *userRepo);
    GUI GUI(app, service);
    GUI.start();
    return GUI.exec();
}

int main(int argc, char *argv[]) {
    return startWithGUI(argc, argv);
}