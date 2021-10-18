# QtClient
# Build and run environment :
# Linux, Ubuntu 18.04
# Qt 5.9 ( Build release of Qt )
# Qt Creator 

# Setup, build and launch the client using QtCreator

# Step 1. Clone the project, git clone https://github.com/joker2904/QtClient.git
# Step 2. Launch Qt Creator and import project ClientGUI.pro
# Step 3. Setup the local build configurations from the Qt Creator ( to be done first time the project is cloned and built on any system ).
# Step 4. Build the application from QtCreator build 
# Step 5: Launch the application.

# Setup, build and launch the client using gmake

# Step 1. Clone the project, git clone https://github.com/joker2904/QtClient.git
# Step 2. cd QtClient
# Step 3. qmake -o Makefile ClientGUI.pro
# Step 4. make
# Step 5. ./ClientGUI

# How to use the client
Make sure the ngrest server is running.
Create players and games by button clicks.
The client has comboboxes to select the game and player ID. Player Id is the authenticator for the player
Select the row and column for this game and click the move button.
The response for any action will be displayed. The client continuously polls the server for the status of games and players and updates the display.
The player status and scores are shown on screen automatically.
