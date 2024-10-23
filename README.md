
# Event Manager Application

## Description
The **Event Manager** is a desktop application developed in **C++** using the **Qt framework**. It allows users to efficiently manage the events they are attending, providing features to add, edit, delete, and categorize events. The application is designed with a user-friendly graphical interface and includes real-time updates to ensure a seamless experience.

## Features
- Add, update, delete, and categorize events.
- Real-time updates to the event list.
- Persistent data storage for saving and loading events across sessions.
- Scalable design for handling multiple events simultaneously.
- Clean, responsive graphical user interface built with **Qt**.
- Implements **Observer** design pattern for UI updates and **MVC** architecture for clean separation of concerns.


## Technologies Used
- **C++**
- **Qt Framework** for the graphical user interface.
- **Observer** design pattern for real-time UI updates.
- **MVC architecture** for modular and maintainable code.

## Prerequisites
- **C++ compiler** (e.g., GCC, Clang)
- **Qt Framework** (Qt 6)
- **CMake**

## Installation
1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/event-manager-app.git
   cd event-manager-app
   ```

2. **Build the project**
 

3. **Run the application**:
   ```bash
   ./EventManagerApp
   ```

## Usage
1. **Start the application** and the main window will display the current list of events.
2. **Add an event** by filling out the event details (name, date, category) and clicking the 'Add' button.
3. **Edit or delete** existing events by selecting them from the list and using the respective buttons.
4. The data is saved automatically, and you can load it in future sessions.


## Contributing
Feel free to submit issues or pull requests. Please ensure any contributions follow the project's code style and are well-documented.
