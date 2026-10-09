package completion;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;


/* This frame is used for setting the paths of elan  *
 * compiler and gmake. */

public class SettingFrame extends JFrame{
    
    /* This field is the java representation of the  *
     * two paths. */
    private Settings settings;

    
    public SettingFrame(Settings set){
	super("Setting");

	settings = set;

	/* Graphic objects */
	JPanel gmake = new JPanel();
	JPanel elan = new JPanel();
	
	JLabel gmakeLabel = new JLabel("path for gmake : ");
	JLabel elanLabel = new JLabel("path for elanc : ");
	
	final JTextField gmakeField = new JTextField(20);
	final JTextField elanField = new JTextField(20);

	final JButton gmakeButton = new JButton("browse");
	final JButton elanButton = new JButton("browse");
	JButton ok = new JButton("Validate");

	gmake.add(gmakeLabel);
	gmake.add(gmakeField);
	gmake.add(gmakeButton);
	elan.add(elanLabel);
	elan.add(elanField);
	elan.add(elanButton);

	Container container = getContentPane();
	container.setLayout(new GridLayout(3,1));
	container.add(gmake);
	container.add(elan);
	container.add(ok);
	
	pack();
	setVisible(true);
	
	/* The action which sets the path either of    *
	 * elanc or gmake, according to the button that*
	 * generated him. */
	ActionListener setPath = new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    JFileChooser fc = new JFileChooser(".");
		    int returnVal;
		    if((returnVal = fc.showOpenDialog(null)) == JFileChooser.APPROVE_OPTION){
			File file = fc.getSelectedFile();
			String path = file.getPath();
			if(e.getSource() == gmakeButton)
			    gmakeField.setText(path);
			else
			    elanField.setText(path);
		    }
		}
	    };
	
	gmakeButton.addActionListener(setPath);
	elanButton.addActionListener(setPath);
	
	/* The ok button will set the Setting object, and*
	 * save it. */
	ok.addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    if(!elanField.getText().equals(""))
			settings.setElan(elanField.getText());
		    if(!gmakeField.getText().equals(""))
			settings.setGmake(gmakeField.getText());
		    try{
			settings.save();
		    }
		    catch(Exception er){
			JOptionPane.showMessageDialog(null, er.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
		    }
		}
	    }
				   );
    }

}
