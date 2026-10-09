package completion;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.util.*;

/* This is the frame containing the critical pairs returned by ELAN. *
 * The pairs are displayed in a JTable. The first and the second     *
 * column contain the two first members which generate the critical  *
 * pair, and the third column contain the substitutions to apply for *
 * having the sames results with the to members.*/

public class CriticalPairsFrame extends JFrame{

    private JTable table;
    private Container container;
    
    /* The vector pairs contain Pair objects which represent the     *
     * the critical pairs. Here, we just create graphical objects and*
     * we set the right values in the table.*/
    public CriticalPairsFrame(Vector pairs, String name){
	super("Critical pairs and superposition for system : " + name);
	container = getContentPane();
	container.setLayout(new BorderLayout(5, 5));
	JPanel panel = new JPanel(new BorderLayout());
	panel.setBorder(BorderFactory.createLineBorder(Color.gray));

	MyTableModel tm = new MyTableModel(0, 5);
	Vector ve = new Vector(5);
	ve.add("first rule");
	ve.add("second rule");
	ve.add("superposition");
	ve.add("first rewrite");;
	ve.add("second rewrite");
	tm.setColumnIdentifiers(ve);
	table = new JTable(tm);
	JScrollPane scrollPane = new JScrollPane(table);
	table.setDefaultRenderer(table.getColumnClass(0), new CpTableCellRenderer());
	
	for(int i = 0; i < pairs.size(); i++){
	    Pair pair = (Pair) pairs.get(i);
	    Vector row = new Vector(5);
	    Vector v = pair.getFirst().doString();
	    v.setElementAt(Integer.toString(pair.getFirstNumber()) + " : " + ((String) v.get(0)), 0); 
	    row.add(v);
	    v = pair.getSecond().doString();
	    v.setElementAt(Integer.toString(pair.getSecondNumber()) + " : " +((String) v.get(0)), 0); 
	    row.add(v);
	    row.add(pair.getSubstitution());
	    row.add(pair.getFirstRewrite().doString());
	    row.add(pair.getSecondRewrite().doString());
	    ((MyTableModel)table.getModel()).addRow(row);
	    table.setRowHeight(i, 20);
	}
	
	if(table.getRowCount() < 10)
	    table.setPreferredScrollableViewportSize(new Dimension(800, (1+table.getRowCount()) * table.getRowHeight()));
	else
	    table.setPreferredScrollableViewportSize(new Dimension(800, 200));
    	JLabel label = new JLabel("Critical Pairs and superposition");
	panel.add(label, BorderLayout.NORTH);
	panel.add(scrollPane, BorderLayout.CENTER);
	container.add(panel);

	pack();
	setVisible(true);
	
    }

    /* A new renderer which changes the string's colors according to their *
     * sources (first or second member).*/

    public class CpTableCellRenderer extends ColoredLabel implements TableCellRenderer {
	
	public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column){
	    if(value != null){
		Vector v = new Vector();
		if(value instanceof Vector){
		    if(((Vector) value).size() > 0 && (((Vector) value).get(0) instanceof Substitution)){
			Vector list = (Vector) value;
			Vector subs = new Vector();
			Vector c = new Vector();
			for(int i = 0; i < list.size(); i++){
			    Substitution s = (Substitution) list.get(i);
			    subs.add(s.getNewName());
			    subs.add("->");
			    subs.add(s.getValue().toStringNewName());
			    subs.add(" ; ");
			    if(s.getMember() == 1){
				c.add(Color.blue);
				c.add(Color.black);
				c.add(new Color(100,200,0));
				c.add(Color.black);
			    }
			    else{
				c.add(new Color(100,200,0));
				c.add(Color.black);
				c.add(Color.blue);
				c.add(Color.black);
			    }
			}
			setText(subs);
			setColor(c);
		    }
		    else if(((Vector) value).size() > 0 && (((Vector) value).get(0) instanceof String)){
			v = (Vector) value;
			setText(v);
		    }
		}
		setOpaque(true);
		setLabelFont(table.getFont());
	    }
	    else{
		setText(null);
	    }
	    Vector color = new Vector();
	    if(column == 0){
		color.add(Color.blue);
		color.add(Color.red);
		color.add(Color.blue);
		setColor(color);
	    }
	    else if(column == 1){
		setColor(new Color(100,200,0));
	    }
	    else if(column == 2){
		if(value != null && value instanceof Vector){
		    Vector txt = new Vector();
		    for(int i = 0; i < ((Vector) value).size(); i++){
			
		    }
		}
	    }
	    else if(column == 3){
		setColor(Color.blue);
	    }
	    else{
		setColor(new Color(100,200,0));
	    }
	    return this;
	}    
    }

}
