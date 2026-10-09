import completion.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class InterfaceElan extends JFrame{

    public InterfaceElan(){
	super("ELAN");
	Container cont = getContentPane();
	SpecificationFrame sf = new SpecificationFrame(this);

	cont.add(sf);
	setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public static void main(String args[]){
	InterfaceElan window = new InterfaceElan();
	window.pack();
	window.setVisible(true);
    }
}
