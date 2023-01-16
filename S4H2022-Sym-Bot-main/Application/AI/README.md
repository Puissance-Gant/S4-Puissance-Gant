# Sym-Bot : AI

## Table of Content
- [Sym-Bot : AI](#sym-bot--ai)
  - [Table of Content](#table-of-content)
  - [Setup](#setup)
  - [Training](#training)
  - [Adding Actions](#adding-actions)

## Setup
- Software : PyCharm (2021.3.1)
- Language : Python (3.9)
- Packages : 
  - numpy (1.22.2)

## Training
Training examples for the AI are stored within the "Symbot_Data_Set.txt" CSV file. The name of the file can be specified in the <i>translate</i> class (<i>Comm/translate.py</i>) under the variable <i>file_data_set</i>.

Before training the AI, make sure the following conditions are met:
- <i>DEVELOPPER_MODE</i> (<i>translate.py</i>) is set to True
- <i>print(msg)</i> lines in <i>main.py</i> are commented out
- <i>N_ACTIONS</i> in <i>translate.init</i> corresponds to the correct amount of possible actions (see [Adding Actions](#adding-actions))
- Connection with glove is ready to be established
- Access to PyCharm terminal (VSCode also works, but the team had issues with it and recommends using PyCharm)

Then, execute the program as normal (from <i>main.py</i>). To add a new training example, follow these steps:
1. Equip the glove
2. Execute the desired hand sign
3. Label the action by entering its corresponding number in the terminal

Here are some tips to improve the efficiency of the AI:
- "Reset" the position of your fingers between every example
- Add some variety to your hand signs (Flex your fingers a bit more or less, rotate your wrist, etc.)
- Make sure the flex sensors readings are reasonable (You can always manually delete a line from the CSV file if necessary)

## Adding Actions
The current implementation of the AI only allows simple instructions in the form of a sequence of hard-coded positions. To add or change an action, simply modify the <i>manageAction</i> function in the <i>translate</i> class accordingly. The sequence is described as follow:
- Sequence : [POSITION_1, POSITION_2, ..., POSITION_N]
- Position : [JOINT_1, JOINT_2, JOINT_3, GRIPPER]

With the following ranges:
- JOINT_1 : [0, 360]
- JOINT_2 : [0, 90]
- JOINT_3 : [0, 90]
- GRIPPER : [0, 180]

Note that to prevent issues with the motors, avoid going to the limits of their corresponding ranges.

To make sure the command is treated accordingly, the value of <i>N_ACTIONS</i> in <i>translate.init</i> should correspond to the maximum value of the <i>elif</i> clause. It's also possible to implement a default <i>else</i> clause to englobe all non-implemented actions.
