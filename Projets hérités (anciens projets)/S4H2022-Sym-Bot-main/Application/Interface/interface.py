#Mettre dans dossier UI

from guizero import App, Text,PushButton,Slider,TextBox

import sys
sys.path.insert(0,"/Users/labmi/Documents/GitHub/Sym-Bot/Application/")
from Comm.MessageIO import MessageIO
from Comm.ControlMessage import ControlMessage
from Comm.SerialComm import SerialComm
import os

class interface:

    gripper = 180

    msgIO = MessageIO()
    # msgIO.addDevice(dev=SerialComm("COM3", 57600))  #Port vers open
    J_mode=True


    def op_grip(self,op,button1,button2):
        self.gripper = 180
        if not op:
            #open_gripper()
            op=1
            button1.image=open_on
            button2.image=closed_off
        print("OPEN")


    def cl_grip(self,op,button1,button2):
        #close_gripper()
        self.gripper = 60
        button1.image=open_off
        button2.image=closed_on
        op=0
        print("CLOSE")
   
    def go_up(self,button,j_id):
        self.currentMsgMotor=[0,0,0,0]
        self.currentMsgMotor[j_id]=1
        msg_to_motor=ControlMessage(ControlMessage.SET_JOG,self.currentMsgMotor)
        while button.value==1:
            self.msgIO.sendMessage(0, msg_to_motor)
            print("1")
        self.currentMsgMotor[j_id]=0
        msg_to_motor=ControlMessage(ControlMessage.SET_JOG,self.currentMsgMotor)
        
    
    def go_down(self,motor,button,j_id):
        self.currentMsgMotor=[0,0,0,0]
        self.currentMsgMotor[j_id]=-1
        msg_to_motor=ControlMessage(ControlMessage.SET_JOG,self.currentMsgMotor)
        while button.value==1:
            self.msgIO.sendMessage(0, msg_to_motor)
            print("-1")
        self.currentMsgMotor[j_id]=0
        msg_to_motor=ControlMessage(ControlMessage.SET_JOG,self.currentMsgMotor)


    def ch_mode(self,M,button1,button2,lab1,lab2,lab3,lab4,lab5,lab6,coord1,coord2,coord3,val1,val2,val3,val4,val5,val6,u1,u2,u3,u4,u5,u6,d1,d2,d3,d4,d5,d6,ch1,ch2,ch3,ch4,ch5,ch6,ca1,ca2,ca3,go1,go2,go3):
        button1.bg=mint
        button2.bg=Peri
        J_mode=M
        lab1.hide()
        lab2.hide()
        lab3.hide()
        lab4.show()
        lab5.show()
        lab6.show()
        coord1.hide()
        coord2.hide()
        coord3.hide()
        val1.hide()
        val2.hide()
        val3.hide()
        val4.show()
        val5.show()
        val6.show()
        u1.hide()
        u2.hide()
        u3.hide()
        u4.show()
        u5.show()
        u6.show()
        d1.hide()
        d2.hide()
        d3.hide()
        d4.show()
        d5.show()
        d6.show()
        ch1.hide()
        ch2.hide()
        ch3.hide()
        ch4.show()
        ch5.show()
        ch6.show()
        ca1.hide()
        ca2.hide()
        ca3.hide()
        go1.hide()
        go2.hide()
        go3.hide()
        
   

    def change_val(self,coord,go_but,ch_but,can_but,u,d):
        ch_but.hide()
        coord.show()
        go_but.show()
        go_but.enable()
        can_but.show()
        can_but.enable()
        u.hide()
        u.disable()
        d.disable()
        d.hide()
        print("MOVE_X")


    def can_but_f(self,coord,go_but,ch_but,can_but,u,d):
        go_but.disable()
        go_but.hide()
        coord.clear()
        coord.hide()
        ch_but.show()
        u.enable()
        u.show()
        d.enable()
        d.show()
        can_but.hide()
        can_but.disable()


    def go_but_f(self,coord,go_but,ch_but,can_but,u,d,text):
        try:
            new_val = float(coord.value)
            go_but.disable()
            go_but.hide()
            coord.bg=Peri
            # coord.clear()
            coord.hide()
            ch_but.show()
            u.enable()
            u.show()
            d.enable()
            d.show()
            can_but.hide()
            can_but.disable()
            text.value=new_val
            # valeurBouton = new_val
        except:
            coord.bg=red

    def sendMsg(self):
        tabMsg = [float(J1_coord.value), float(J2_coord.value), float(J3_coord.value), self.gripper]
        msg_to_motor = ControlMessage(ControlMessage.SET_JOINT_POSITION, tabMsg)
        self.msgIO.sendMessage(0, msg_to_motor)
        print(tabMsg)    

if __name__ == '__main__':
    
    obj = interface()
    app=App(title="Symbot",layout="grid",width=600,height=550)
    C_Title=Text(app,text="Coordinates",grid=[25,50])
    red=(255,0,0)
    green=(0,255,0)
    cer=(86,87,255)
    mint=(156,255,154)
    Peri=(155,156,255)
    app.bg=Peri
    open_on="Application/Interface/Boutonpinceouverteonp.png"
    open_off="Application/Interface/Boutonpinceouverteoffp.png"
    closed_on="Application/Interface/Boutonpincefermeeonp.png"
    closed_off="Application/Interface/Boutonpincefermeep.png"
    is_op=False
    


    #grip_com=ButtonGroup(app,command=op_cl_grip,options)

    cart_mode_but=PushButton(app,text="Cartesian Mode",grid=[20,0])
    joint_mode_but=PushButton(app,text="Joint Mode",grid=[20,10])
    op_grip_but=PushButton(app,image=open_on,grid=[100,0],width=108,height=97)
    cl_grip_but=PushButton(app,image=closed_off,grid=[100,10],width=108,height=97)
    op_grip_but.update_command(obj.op_grip,args=(is_op,op_grip_but,cl_grip_but))
    cl_grip_but.update_command(obj.cl_grip,args=(is_op,op_grip_but,cl_grip_but))
    # cart_mode_but.update_command(obj.ch_mode,args=(False,cart_mode_but,joint_mode_but,J1_lab,J2_lab,J3_lab,X_lab,Y_lab,Z_lab,J1_coord,J2_coord,J3_coord,J1_val,J2_val,J3_val,X_val,Y_val,Z_val,J1_U,J2_U,J3_U,X_U,Y_U,Z_U,J1_D,J2_D,J3_D,X_D,Y_D,Z_D,J1_ch,J2_ch,J3_ch,X_ch,Y_ch,Z_ch,J1_Ca,J2_Ca,J3_Ca,J1_Go,J2_Go,J3_Go))
    # joint_mode_but.update_command(obj.ch_mode,args=(True,joint_mode_but,cart_mode_but,X_lab,Y_lab,Z_lab,J1_lab,J2_lab,J3_lab,X_coord,Y_coord,Z_coord,X_val,Y_val,Z_val,J1_val,J2_val,J3_val,X_U,Y_U,Z_U,J1_U,J2_U,J3_U,X_D,Y_D,Z_D,J1_D,J2_D,J3_D,X_ch,Y_ch,Z_ch,J1_ch,J2_ch,J3_ch,X_Ca,Y_Ca,Z_Ca,X_Go,Y_Go,Z_Go))
    X_lab=Text(app,text="X",grid=[0,60],align="left",visible=False)
    Y_lab=Text(app,text="Y",grid=[0,70],align="left",visible=False)
    Z_lab=Text(app,text="Z",grid=[0,80],align="left",visible=False)
    J1_lab=Text(app,text="J1",grid=[0,60],align="left")
    J2_lab=Text(app,text="J2",grid=[0,70],align="left")
    J3_lab=Text(app,text="J3",grid=[0,80],align="left")
    X_val=Text(app,text="???",grid=[10,60],align="left",visible=False)
    Y_val=Text(app,text="???",grid=[10,70],align="left",visible=False)
    Z_val=Text(app,text="???",grid=[10,80],align="left",visible=False)
    J1_val=Text(app,text="???",grid=[10,60],align="left")
    J2_val=Text(app,text="???",grid=[10,70],align="left")
    J3_val=Text(app,text="???",grid=[10,80],align="left")
    X_coord=TextBox(app,grid=[25,60],visible=False)
    Y_coord=TextBox(app,grid=[25,70],visible=False)
    Z_coord=TextBox(app,grid=[25,80],visible=False)
    J1_coord=TextBox(app,grid=[25,60],visible=False)
    J2_coord=TextBox(app,grid=[25,70],visible=False)
    J3_coord=TextBox(app,grid=[25,80],visible=False)
    X_U=PushButton(app,text="▲",grid=[25,59],visible=False)
    X_D=PushButton(app,text="▼",grid=[25,60],visible=False)
    Y_U=PushButton(app,text="▲",grid=[25,69],visible=False)
    Y_D=PushButton(app,text="▼",grid=[25,70],visible=False)
    Z_U=PushButton(app,text="▲",grid=[25,79],visible=False)
    Z_D=PushButton(app,text="▼",grid=[25,80],visible=False)
    J1_U=PushButton(app,text="▲",grid=[25,59])
    J1_D=PushButton(app,text="▼",grid=[25,60])
    J2_U=PushButton(app,text="▲",grid=[25,69])
    J2_D=PushButton(app,text="▼",grid=[25,70])
    J3_U=PushButton(app,text="▲",grid=[25,79])
    J3_D=PushButton(app,text="▼",grid=[25,80])
    X_U.text_size=4
    X_D.text_size=4
    Y_U.text_size=4
    Y_D.text_size=4
    Z_U.text_size=4
    Z_D.text_size=4
    J1_U.text_size=4
    J1_D.text_size=4
    J2_U.text_size=4
    J2_D.text_size=4
    J3_U.text_size=4
    J3_D.text_size=4
    J1_U.update_command(obj.go_up,args=(J1_U,1))
    J1_D.update_command(obj.go_down,args=(J1_D,1))
    J2_U.update_command(obj.go_up,args=(J2_U,2))
    J1_D.update_command(obj.go_down,args=(J2_D,2))
    J3_U.update_command(obj.go_up,args=(J3_U,3))
    J3_D.update_command(obj.go_down,args=(J3_D,3))

    X_ch=PushButton(app,text="Move",grid=[50,60],visible=False)
    X_Go=PushButton(app,text="Go",grid=[50,60],enabled=False,visible=False)
    X_Ca=PushButton(app,text="Cancel",grid=[75,60],enabled=False,visible=False)
    Y_ch=PushButton(app,text="Move",grid=[50,70],visible=False)
    Y_Go=PushButton(app,text="Go",grid=[50,70],enabled=False,visible=False)
    Y_Ca=PushButton(app,text="Cancel",grid=[75,70],enabled=False,visible=False)
    Z_ch=PushButton(app,text="Move",grid=[50,80],visible=False)
    Z_Go=PushButton(app,text="Go",grid=[50,80],enabled=False,visible=False)
    Z_Ca=PushButton(app,text="Cancel",grid=[75,80],enabled=False,visible=False)
    J1_ch=PushButton(app,text="Move",grid=[50,60])
    J1_Go=PushButton(app,text="Go",grid=[50,60],enabled=False,visible=False)
    J1_Ca=PushButton(app,text="Cancel",grid=[75,60],enabled=False,visible=False)
    J2_ch=PushButton(app,text="Move",grid=[50,70])
    J2_Go=PushButton(app,text="Go",grid=[50,70],enabled=False,visible=False)
    J2_Ca=PushButton(app,text="Cancel",grid=[75,70],enabled=False,visible=False)
    J3_ch=PushButton(app,text="Move",grid=[50,80])
    J3_Go=PushButton(app,text="Go",grid=[50,80],enabled=False,visible=False)
    J3_Ca=PushButton(app,text="Cancel",grid=[75,80],enabled=False,visible=False)

    X_ch.update_command(obj.change_val,args=(X_coord,X_Go,X_ch,X_Ca,X_U,X_D))
    X_Go.update_command(obj.go_but_f,args=(X_coord,X_Go,X_ch,X_Ca,X_U,X_D,X_val,))
    X_Ca.update_command(obj.can_but_f,args=(X_coord,X_Go,X_ch,X_Ca,X_U,X_D))
    Y_ch.update_command(obj.change_val,args=(Y_coord,Y_Go,Y_ch,Y_Ca,Y_U,Y_D))
    Y_Go.update_command(obj.go_but_f,args=(Y_coord,Y_Go,Y_ch,Y_Ca,Y_U,Y_D,Y_val))
    Y_Ca.update_command(obj.can_but_f,args=(Y_coord,Y_Go,Y_ch,Y_Ca,Y_U,Y_D))
    Z_ch.update_command(obj.change_val,args=(Z_coord,Z_Go,Z_ch,Z_Ca,Z_U,Z_D))
    Z_Go.update_command(obj.go_but_f,args=(Z_coord,Z_Go,Z_ch,Z_Ca,Z_U,Z_D,Z_val))
    Z_Ca.update_command(obj.can_but_f,args=(Z_coord,Z_Go,Z_ch,Z_Ca,Z_U,Z_D))
    J1_ch.update_command(obj.change_val,args=(J1_coord,J1_Go,J1_ch,J1_Ca,J1_U,J1_D))
    J1_Go.update_command(obj.go_but_f,args=(J1_coord,J1_Go,J1_ch,J1_Ca,J1_U,J1_D,J1_val))
    J1_Ca.update_command(obj.can_but_f,args=(J1_coord,J1_Go,J1_ch,J1_Ca,J1_U,J1_D))
    J2_ch.update_command(obj.change_val,args=(J2_coord,J2_Go,J2_ch,J2_Ca,J2_U,J2_D))
    J2_Go.update_command(obj.go_but_f,args=(J2_coord,J2_Go,J2_ch,J2_Ca,J2_U,J2_D,J2_val))
    J2_Ca.update_command(obj.can_but_f,args=(J2_coord,J2_Go,J2_ch,J2_Ca,J2_U,J2_D))
    J3_ch.update_command(obj.change_val,args=(J3_coord,J3_Go,J3_ch,J3_Ca,J3_U,J3_D))
    J3_Go.update_command(obj.go_but_f,args=(J3_coord,J3_Go,J3_ch,J3_Ca,J3_U,J3_D,J3_val))
    J3_Ca.update_command(obj.can_but_f,args=(J3_coord,J3_Go,J3_ch,J3_Ca,J3_U,J3_D))

    cart_mode_but.update_command(obj.ch_mode,args=(False,cart_mode_but,joint_mode_but,J1_lab,J2_lab,J3_lab,X_lab,Y_lab,Z_lab,J1_coord,J2_coord,J3_coord,J1_val,J2_val,J3_val,X_val,Y_val,Z_val,J1_U,J2_U,J3_U,X_U,Y_U,Z_U,J1_D,J2_D,J3_D,X_D,Y_D,Z_D,J1_ch,J2_ch,J3_ch,X_ch,Y_ch,Z_ch,J1_Ca,J2_Ca,J3_Ca,J1_Go,J2_Go,J3_Go))
    joint_mode_but.update_command(obj.ch_mode,args=(True,joint_mode_but,cart_mode_but,X_lab,Y_lab,Z_lab,J1_lab,J2_lab,J3_lab,X_coord,Y_coord,Z_coord,X_val,Y_val,Z_val,J1_val,J2_val,J3_val,X_U,Y_U,Z_U,J1_U,J2_U,J3_U,X_D,Y_D,Z_D,J1_D,J2_D,J3_D,X_ch,Y_ch,Z_ch,J1_ch,J2_ch,J3_ch,X_Ca,Y_Ca,Z_Ca,X_Go,Y_Go,Z_Go))

    # S_title=Text(app,text="Sensors",grid=[175,50])
    # p_lab=Text(app,text="Pinky",grid=[150,90],align="left")
    # i_lab=Text(app,text="Index",grid=[150,60],align="left")
    # m_lab=Text(app,text="Middle",grid=[150,70],align="left")
    # r_lab=Text(app,text="Ring",grid=[150,80],align="left")
    # o_lab=Text(app,text="IMU",grid=[150,100],align="left")

    # p_sl=Slider(app,grid=[175,90])
    # i_sl=Slider(app,grid=[175,60])
    # m_sl=Slider(app,grid=[175,70])
    # r_sl=Slider(app,grid=[175,80])
    # o_sl=Slider(app,grid=[175,100])

    # i_cl=PushButton(app,text="Set as closed",grid=[225,60])
    # m_cl=PushButton(app,text="Set as closed",grid=[225,70])
    # r_cl=PushButton(app,text="Set as closed",grid=[225,80])
    # p_cl=PushButton(app,text="Set as closed",grid=[225,90])
    # i_op=PushButton(app,text="Set as open",grid=[200,60])
    # m_op=PushButton(app,text="Set as open",grid=[200,70])
    # r_op=PushButton(app,text="Set as open",grid=[200,80])
    # p_op=PushButton(app,text="Set as open",grid=[200,90])

    ###############################################################################
    ## Bouton ajoutee
    ###############################################################################
    sendButton=PushButton(app,obj.sendMsg,text="Send command",grid=[22,250])

    # i_cl.bg=cer
    # m_cl.bg=cer
    # r_cl.bg=cer
    # p_cl.bg=cer
    # i_op.bg=mint
    # m_op.bg=mint
    # r_op.bg=mint
    # p_op.bg=mint

    ###############################################################################
    ## Show App
    ###############################################################################
    app.display()

    ###############################################################################
    ## Gestion des messages
    ###############################################################################

