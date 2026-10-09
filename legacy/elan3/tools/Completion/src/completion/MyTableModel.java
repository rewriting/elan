package completion;

import javax.swing.table.*;

/* This tableModel is used for not editables Jtables */
public class MyTableModel extends DefaultTableModel{
    
    public MyTableModel(int row, int column){
	super(row, column);
    }
    
    public boolean isCellEditable(int row, int column){
	return false;
    }

    

    

}
