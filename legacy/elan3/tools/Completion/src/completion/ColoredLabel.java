package completion;

import java.awt.*;
import javax.swing.*;
import java.util.*;


/* This class has only a graphic interest. It is used *
 * to display several colors of font in a JLabel.     *
 * In fact it isn't a JLabel but a JPanel containing  *
 * several JLabels. */

public class ColoredLabel extends JPanel {

    /* Fields */

    /* A vector of String is the text to display */
    private Vector text;

    /* The number of labels. */
    private int nbLabels;

    /* A vector of font colors. */
    private Vector colors;

    /* A vector of labels. */
    private Vector labels;
    
    public ColoredLabel(){
	this(1, null, null);
    }

    public ColoredLabel(Vector t){
	this(t.size(), null, t);
    }
    
    public ColoredLabel(int n, Vector c, Vector t){
	super(new FlowLayout());
	nbLabels = n;
	labels = new Vector(nbLabels);
	for(int i = 0; i < nbLabels; i++)
	    labels.add(new JLabel());
	
	if(c == null)
	    setColor(Color.black);
	else
	    setColor(c);
	setBackground(Color.white);
	setText(t);
	
	
	

	for(int i = 0; i < nbLabels; i++)
	    add((JLabel) labels.get(i));
    }

    public void setColor(Vector c){
	colors = c;
	for(int i = 0; i < nbLabels; i++){
	    ((JLabel) labels.get(i)).setForeground(((Color) colors.get(i)));
	}
    }
    
    public void setColor(Color c){
	if(colors == null || colors.size() != nbLabels)
	    colors = new Vector(nbLabels);
	for(int i = 0; i < nbLabels; i++){
	    colors.add(c);
	    ((JLabel) labels.get(i)).setForeground(((Color) colors.get(i)));
	}
    }

    public void setNbLabels(int n){
	nbLabels = n;
	labels = new Vector(n);
	for(int i = 0; i < nbLabels; i++){
	    labels.add(new JLabel());
	}
    }
    
    public void setLabelFont(Font f){
	for(int i = 0; i < nbLabels; i++)
	    ((JLabel) labels.get(i)).setFont(f);
    }
    
    public void setText(Vector v){
	text = v;
	removeAll();
	if(text != null && text.size() > 0){
	    if(text.size() != nbLabels){
		nbLabels = text.size();
		labels = new Vector(nbLabels);
		for(int i = 0; i < nbLabels; i++){
		    labels.add(new JLabel((String) text.get(i)));
		}
	    }
	    else{
		for(int i = 0; i < nbLabels; i++)
		    ((JLabel) labels.get(i)).setText((String) text.get(i));
	    }
	    setColor(Color.black);
	}
	else{
	    for(int i = 0; i < nbLabels; i++)
		((JLabel) labels.get(i)).setText("");
	}
	refresh();
    }

    private void refresh(){
	for(int i = 0; i < nbLabels; i++)
	    add((JLabel) labels.get(i));
    }
}
