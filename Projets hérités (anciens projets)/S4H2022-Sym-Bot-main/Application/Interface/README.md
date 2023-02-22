This interface was made using guizero. You can find information on how to download and use guizero here: https://lawsie.github.io/guizero/ Further information about how to add and configure widgets can be found under the widget tab of that website.

The point of this interface is to test out functions on the arm without the glove and vice-versa. This is done using buttons, which send commands to the arm. The move button next to the coordinates allows the user to input a coordinate of his choice. Once the coordinates have been registered, the user may send them using the send button. The open and close gripper buttons glow according to the state of the gripper. The cartesian/joint mode buttons allow to switch states (theoretically).

The images in this folder are important. They are the images for the gripper buttons. If you delete them, the code will no longer be able to access them and the buttons will be blank.

Ideally, there would be a retroaction between the state of the arm and the interface. Ideally, there would also be a way to show the state of the glove on the interface.
