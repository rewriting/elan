package elan;
import java.awt.*;


public class LabelText{

    private String text;
    private Color color;

    public LabelText(String txt, Color c){
	text = txt;
	color = c;
    }

     public LabelText(String txt){
	 this(txt,Color.BLACK);
    }

    public String getText(){
	return text;
    }
    
    public Color getColor(){
	return color;
    }
}
