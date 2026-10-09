package completion;

//import elan.completion.*;
import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.util.*;

/* This is the frame containing the oriented rules returned by ELAN. *
 * The rules are displayed in a JTable. The first and the third      *
 * column contain respectively the left part and the right part of   *
 * the rule.*/

public class OrientationFrame extends JFrame{

    private JTable table;
    private Container container;
    private Vector rules;

    /* The argument is a vector of Rule, which is the java object    *
     * representing equalities or oriented rules.*/
    public OrientationFrame(Vector rules, String name){
	super("Orientation of rules of system : " + name);
	this.rules = rules;
	container = getContentPane();
	container.setLayout(new BorderLayout(5, 5));
	JPanel panel = new JPanel(new BorderLayout());
	panel.setBorder(BorderFactory.createLineBorder(Color.gray));

	MyTableModel tm = new MyTableModel(0, 3);
	Vector ve = new Vector(3);
	ve.add("");
	ve.add("");
	ve.add("");
	tm.setColumnIdentifiers(ve);
	table = new JTable(tm);
	
	TableColumn column;
	for(int i = 0; i < 3; i++){
	    column = table.getColumnModel().getColumn(i);
	    if(i == 1){
		column.setPreferredWidth(20);
	    } 
	    else{
		column.setPreferredWidth(360);
	    }
	}
	JScrollPane scrollPane = new JScrollPane(table);
	table.setDefaultRenderer(table.getColumnClass(0), new OrientTableCellRenderer());

	
	for(int i = 0; i < rules.size(); i++){
	    Rule rule = (Rule) rules.get(i);
	    Vector row = new Vector(3);
	    row.add(rule.getLeft());
	    row.add("->");
	    row.add(rule.getRight());
	    ((MyTableModel)table.getModel()).addRow(row);
	    table.setRowHeight(i, 20);
	}

	if(table.getRowCount() < 10)
	    table.setPreferredScrollableViewportSize(new Dimension(800, (1+table.getRowCount()) * table.getRowHeight()));
	else
	    table.setPreferredScrollableViewportSize(new Dimension(800, 200));
	JLabel label = new JLabel("Orientation");
	panel.add(label, BorderLayout.NORTH);
	panel.add(scrollPane, BorderLayout.CENTER);
	container.add(panel);

	pack();
	setVisible(true);
    }
 
    /* A new renderer with which the rules that have changed are highlighted.*/
    public class OrientTableCellRenderer extends JLabel implements TableCellRenderer {
	
	public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column){
	    if(value != null){
		String s = value.toString();
		setText(s);
		if(column != 1){
		    //if(.getValueAt(row, column) != null){
			if(((OpTree) value).hasFlag())
			    setBackground(Color.red);
 			else
 			    setBackground(Color.white);
// 			if(! s.equals(system.getValueAt(row, column).toString()))
// 			    setBackground(Color.red);
// 			else
// 			    setBackground(Color.white);
			//    }
		}
		setOpaque(true);
		setFont(table.getFont());
	    }
	    return this;
	}
    }
}
