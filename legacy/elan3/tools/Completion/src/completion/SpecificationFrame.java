package completion;


import completion.parser.*;

import java.io.*;
import java.util.*;
import java.util.regex.*;
import javax.swing.*;
import javax.swing.table.*;
import javax.swing.event.*;
import javax.swing.filechooser.*;
import java.awt.*;
import java.awt.event.*;


public class SpecificationFrame extends JPanel{ 
    
    private String pathName = null;
    private String pathElan = null;
    private Specification spec = null;
    private Settings settings = null;

       
    private boolean error = false;
    private boolean fileSaved = false;
    private boolean cpCompiled = false;
    private boolean orientCompiled = false;
    private boolean completionCompiled = false;

    private File file;
    private JFileChooser explor;
    private JFrame mainFrame;
    private JScrollPane scrollPane;
    private JScrollPane scrollPaneSyst;	
    private JTextField theVars;
    private JTextArea thePrec;
    private JTable theSyst;

    private static final int OPEN = 1;
    private static final int SAVE = 2;


    public SpecificationFrame(JFrame frame){
	super(new BorderLayout(10,10));
	//	setBorder(BorderFactory.createLoweredBevelBorder());
	//setBorder(BorderFactory.createRaisedBevelBorder());
	mainFrame = frame;
	fileSaved = false;
	loadSettings();
	    
	

	JPanel variables = new JPanel(new GridLayout(2, 1));
	JPanel operators = new JPanel(new BorderLayout());
	JPanel system = new JPanel(new BorderLayout());
	JPanel buttons = new JPanel(new FlowLayout());
	JPanel specification = new JPanel(new BorderLayout());

	
	JLabel vars = new JLabel("Vars");
	JLabel ops = new JLabel("Operators' definition and precedance :");
	JLabel prec = new JLabel("Precedance");
	JLabel syst = new JLabel("System");

	JButton valid = new JButton("Validation");
	JButton pair = new JButton("CP");
	JButton orient = new JButton("Orient");
	JButton comp = new JButton("Completion");

	buttons.add(valid);
	buttons.add(pair);
	buttons.add(orient);
	buttons.add(comp);

	theVars = new JTextField();
	thePrec = new JTextArea(5,30);
	JScrollPane scrollPrec = new JScrollPane(thePrec);

	JMenuBar bar = new JMenuBar();
	final JMenu fileMenu = new JMenu("File");
 	JMenu compileMenu = new JMenu("Compile");
	JMenu preferenceMenu = new JMenu("Preference");
	fileMenu.add(new JMenuItem("New"));
	fileMenu.add(new JMenuItem("Open"));
	fileMenu.add(new JMenuItem("Save"));
	compileMenu.add(new JMenuItem("Critical Pairs"));
	compileMenu.add(new JMenuItem("Orientation"));
	compileMenu.add(new JMenuItem("Completion"));
	preferenceMenu.add(new JMenuItem("set path"));
	
	bar.add(fileMenu);
	bar.add(compileMenu);
	bar.add(preferenceMenu);
	bar.setBorderPainted(true);

	final DefaultTableModel tmSyst = new DefaultTableModel(15,4);
	Vector v = new Vector();
       	v.add("");
	v.add("");
	v.add("");
	v.add("");
	tmSyst.setColumnIdentifiers(v);
	theSyst = new JTable(tmSyst);
	theSyst.setPreferredScrollableViewportSize(new Dimension(600, 200));
	scrollPaneSyst = new JScrollPane(theSyst);
	ListSelectionModel rowSMSyst = theSyst.getSelectionModel();
	for(int i = 0; i < theSyst.getRowCount(); i++){
	    theSyst.setValueAt(Integer.toString(i+1), i, 0);
	    theSyst.setValueAt("=",i,2);
	}
	TableColumn column;
	for(int i = 0; i < 4; i++){
	    column = theSyst.getColumnModel().getColumn(i);
	    if(i == 0 || i == 2){
		column.setPreferredWidth(20);
	    } 
	    else{
		column.setPreferredWidth(280);
	    }
	}

	mainFrame.setJMenuBar(bar);
	
	variables.add(vars);
	variables.add(theVars);
	variables.setBorder(BorderFactory.createLineBorder(Color.gray));

 	operators.add(ops, BorderLayout.NORTH);
 	operators.add(scrollPrec, BorderLayout.CENTER);
 	operators.setBorder(BorderFactory.createLineBorder(Color.gray));


	system.add(syst, BorderLayout.NORTH);
	system.add(scrollPaneSyst, BorderLayout.CENTER);
	system.setBorder(BorderFactory.createLineBorder(Color.gray));

	specification.add(variables, BorderLayout.NORTH);
	specification.add(operators, BorderLayout.CENTER);
	specification.add(system, BorderLayout.SOUTH);
	
	add(specification, BorderLayout.NORTH);
	add(buttons, BorderLayout.CENTER);

	rowSMSyst.addListSelectionListener(new ListSelectionListener(){
		public void valueChanged(ListSelectionEvent e){
		    ListSelectionModel lsm = (ListSelectionModel) e.getSource();
		    int r;
		    if((r = lsm.getMinSelectionIndex()) == theSyst.getRowCount()-1){
			Vector row = new Vector(4);
			row.add(0, Integer.toString(theSyst.getRowCount()+1));
			row.add(1, "");
			row.add(2, "=");
			row.add(3, "");
    			tmSyst.addRow(row);
		    }
		    fileSaved = false;
		}
	    });

	thePrec.addCaretListener(new CaretListener(){
		public void caretUpdate(CaretEvent e){
		    fileSaved = false;
		}
	    }
				  );

	theVars.addCaretListener(new CaretListener(){
		public void caretUpdate(CaretEvent e){
		    fileSaved = false;
		}
	    }
				  );

	fileMenu.getItem(0).addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    clean(theSyst);
		    theVars.setText("");
		    thePrec.setText("");
		}
	    }
			       );


	/* The user can open a .kbc file */
	explor = new JFileChooser(".");
	explor.setFileFilter(new KbcFileFilter());
	fileMenu.getItem(1).addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    int returnVal;
		    if((returnVal = explor.showOpenDialog(null)) == JFileChooser.APPROVE_OPTION){
			try{
			    clean(theSyst);
			    thePrec.setText("");
			    theVars.setText("");
			    File file = explor.getSelectedFile();
			    pathName = file.getPath();
			    
			    FileReader fr = new FileReader(file);
			    BufferedReader br = new BufferedReader(fr);
			    String txt;
			    while((txt = br.readLine()) != null){
				if(txt.equals("vars"))
				    theVars.setText(br.readLine());
				if(txt.equals("prec")){
				    while(! (txt = br.readLine()).equals("syst"))
					thePrec.append(txt + "\n");
				}
				if(txt.equals("syst")){
				    for(int i = 0;(txt = br.readLine()) != null; i++){
					String[] eq = txt.split("=");
					if(theSyst.getRowCount() < i+1){
					    Vector row = new Vector(4);
					    // row.add(Integer.toString(i));
					    row.add(Integer.toString(i+1));
					    row.add(eq[0]);
					    row.add("=");
					    row.add(eq[1]);
					    ((DefaultTableModel)theSyst.getModel()).addRow(row);
					}
					else{
					    theSyst.setValueAt(Integer.toString(i+1), i, 0);
					    theSyst.setValueAt(eq[0], i, 1);
					    theSyst.setValueAt("=", i, 2);
					    theSyst.setValueAt(eq[1], i, 3);
					}
				    }
				}
			    }
			    
			    pathElan = pathName.replaceFirst(".kbc", ".spc");
			    mainFrame.setTitle("ELAN : " + pathName);
			    validate();
			    spec = new Specification(file, OPEN);
			    if(!spec.getWarning().equals("")){
				JOptionPane.showMessageDialog(null, spec.getWarning(), "Warning !", JOptionPane.WARNING_MESSAGE);
			    }
			    else
				fileSaved = true;
			    orientCompiled = false;
			    cpCompiled = false;
			    completionCompiled = false;
			}
			catch(KbcException er){
			    JOptionPane.showMessageDialog(null, "can't generate the specification : " + er.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
			}
			catch(Exception err){
			    JOptionPane.showMessageDialog(null, "can't generate the specification : " + err.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
			}
		    }
		}
	    }
			       );

	

	/* The user, by clicking the 'save' button creates a new
	 * '.spc' file containing the specification. This file 
	 * will be used by elan */
	//	save.addActionListener(new ActionListener(){
	
	
	ActionListener validation = new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    int returnVal;
		    if((returnVal = explor.showSaveDialog(null)) == JFileChooser.APPROVE_OPTION){
			try{
			    error = false;
			    File f = explor.getSelectedFile();
			    FileWriter fw = new FileWriter(f);
			    String txt;
			    fw.write("vars\n");
			    fw.write(theVars.getText() + "\n");
			    fw.write("prec\n");
			    fw.write(thePrec.getText() + "\n");
			    fw.write("syst\n");
			    for(int i = 0; i < theSyst.getRowCount(); i++){
				if(theSyst.getValueAt(i, 1) != null && !((String) theSyst.getValueAt(i, 1)).equals("")){
				    fw.write((String) theSyst.getValueAt(i, 1) + "=" + (String) theSyst.getValueAt(i, 3) + "\n");
				}
			    }
			    fw.close();
			    pathName = f.getPath();
			    pathElan = pathName.replaceFirst(".kbc", ".spc");
			    mainFrame.setTitle("ELAN : " + pathName);
 			    spec = new Specification(f, SAVE);
			    fileSaved = true;
			    if(!spec.getWarning().equals("")){
				JOptionPane.showMessageDialog(new JFrame(), spec.getWarning(), "Warning !", JOptionPane.WARNING_MESSAGE);
			    }
			    fileSaved = true;
			    orientCompiled = false;
			    cpCompiled = false;
			    completionCompiled = false;
			}
			catch(Exception er){
			    JOptionPane.showMessageDialog(null, "can't save file : " + er.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
			    error = true;
			}
		    }
		    else
			error = true;
		}
	    };
	    
	fileMenu.getItem(2).addActionListener(validation);		       
	valid.addActionListener(validation);





	ActionListener critic =  new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    Runtime run = Runtime.getRuntime();
 		    try{
			Process p;
			InputStreamReader isr;
			BufferedReader br;
			String output;;
			InputStreamReader er;
			BufferedReader ber;
			String err = null, listError = "";
			// create competion executable: cp.out
			if(!cpCompiled){
			    p = run.exec("rm -f critical_pairs.make cp.out");
			    if(! fileSaved)
				fileMenu.getItem(2).doClick();
			    if(error){
				throw(new CompilationException("error before compiling."));
			    }
			    p = run.exec(settings.getElan() + " -output cp.out critical_pairs.lgi " + pathElan);
			    isr = new InputStreamReader(p.getInputStream());
			    br = new BufferedReader(isr);
			    er = new InputStreamReader(p.getErrorStream());
			    ber = new BufferedReader(er);
			
			
			    while((err = ber.readLine()) != null)
				listError += err;
			    ber.close();
			    if(! listError.equals(""))
				throw(new CompilationException("gmake " + listError));
			    while((output = br.readLine()) != null)
				System.out.println(output);
			    br.close();

			    p = run.exec(settings.getGmake() + " --file critical_pairs.make");
			    isr = new InputStreamReader(p.getInputStream());
			    br = new BufferedReader(isr);

			    while((output = br.readLine()) != null)
				System.out.println(output);
			    br.close();
			}
			
			p = run.exec("./cp.out");
			isr = new InputStreamReader(p.getInputStream());
			br = new BufferedReader(isr);
			OutputStreamWriter osw = new OutputStreamWriter(p.getOutputStream());
			osw.write("getCP end \n");
			osw.close();
			
  			while((output = br.readLine()) != null){
   			    if(output.startsWith("result")){
				Parser parser = new Parser(new ByteArrayInputStream(output.getBytes()));
				spec.setCriticalPairs(parser.inputCP());
				CriticalPairsFrame cpf = new CriticalPairsFrame(spec.getCriticalPairs(), pathName);
				break;
			    }
			}

			br.close();
			cpCompiled = true;
		    }
		    catch(CompilationException ce){
			JOptionPane.showMessageDialog(null, "can't compile file : " + ce.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
		    }
		    catch(Exception er){
			JOptionPane.showMessageDialog(null, "can't do CP : " + er.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
		    }
		}
	    };
	
	compileMenu.getItem(0).addActionListener(critic);
	pair.addActionListener(critic);

	ActionListener or = new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    Runtime run = Runtime.getRuntime();
		    

		    try{
			Process p;
			InputStreamReader isr;
			BufferedReader br;
			String output;;
			InputStreamReader er;
			BufferedReader ber;
			String error = null, listError = "";

			if(! orientCompiled){
			    p = run.exec("rm -f orient.make orient.out");
			    if(! fileSaved)
				fileMenu.getItem(2).doClick();
			    p = run.exec(settings.getElan() + " -output orient.out orient.lgi " + pathElan);
			    isr = new InputStreamReader(p.getInputStream());
			    br = new BufferedReader(isr);
			    er = new InputStreamReader(p.getErrorStream());
			    ber = new BufferedReader(er);
			    while((error = ber.readLine()) != null)
				listError += error;
			    ber.close();
			    if(! listError.equals(""))
				throw(new CompilationException(listError));
			
			    while((output = br.readLine()) != null)
				System.out.println(output);
			    br.close();

 			    //System.out.println("start make");
			    p = run.exec(settings.getGmake() + " --file orient.make");
 			    //System.out.println("end make");
			    isr = new InputStreamReader(p.getInputStream());
			    br = new BufferedReader(isr);
			    while((output = br.readLine()) != null)
				System.out.println(output);
			    br.close();
			}
 			
                        p = run.exec("./orient.out");
                        isr = new InputStreamReader(p.getInputStream());
                        br = new BufferedReader(isr);
			OutputStreamWriter osw = new OutputStreamWriter(p.getOutputStream());
                        osw.write("getOrient end \n");
			osw.close();
                        
                        while((output = br.readLine()) != null){
                            System.out.println("Output: " + output);
                            if(output.startsWith("result")){
                                Parser parser = new Parser(new ByteArrayInputStream(output.getBytes()));
                                spec.setOrientedRules(parser.inputOrient());
                                OrientationFrame of = new OrientationFrame(spec.getOrientedRules(), pathName);
                                break;
                            }
                        }	
                        br.close();
                        
			orientCompiled = true;
		    } 
		    catch(CompilationException ce){
			JOptionPane.showMessageDialog(null, "can't compile file : " + ce.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
		    }
		    catch(Exception er){
			JOptionPane.showMessageDialog(null, "can't do orient : " + er.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
		    }
		}
	    };				 
	
	compileMenu.getItem(1).addActionListener(or);
	orient.addActionListener(or);
	
	ActionListener co = new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    Runtime run = Runtime.getRuntime();
		    try{
			Process p;
			InputStreamReader isr;
			BufferedReader br;
			String output;;
			InputStreamReader er;
			BufferedReader ber;
			String error = null, listError = "";

			if(! completionCompiled){
			    p = run.exec("rm -f ans_completion.make completion.out");
			    if(! fileSaved)
				fileMenu.getItem(2).doClick();
			    p = run.exec(settings.getElan() + " -output completion.out ans_completion.lgi " + pathElan);
			    isr = new InputStreamReader(p.getInputStream());
			    br = new BufferedReader(isr);
			    er = new InputStreamReader(p.getErrorStream());
			    ber = new BufferedReader(er);
			    while((error = ber.readLine()) != null)
				listError += error;
			    ber.close();
			    if(! listError.equals(""))
				throw(new CompilationException(listError));
			    while((output = br.readLine()) != null)
				System.out.println(output);
			    br.close();

			    p = run.exec(settings.getGmake() + " --file ans_completion.make");
			    isr = new InputStreamReader(p.getInputStream());
			    br = new BufferedReader(isr);
			    while((output = br.readLine()) != null)
				System.out.println(output);
			    br.close();
			}
			p = run.exec("./completion.out");
			isr = new InputStreamReader(p.getInputStream());
			br = new BufferedReader(isr);
			OutputStreamWriter osw = new OutputStreamWriter(p.getOutputStream());
			osw.write("sat end \n");
			osw.close();
			while((output = br.readLine()) != null){
   			    if(output.startsWith("result")){
				break;
			    }
			}
			Parser parser = new Parser(new ByteArrayInputStream(output.getBytes()));
			spec.setCompletion(parser.inputCompletion());
			CompletionFrame cf = new CompletionFrame(spec.getCompletion(), pathName);
			br.close();
			completionCompiled = true;
		    }
		    catch(CompilationException ce){
			JOptionPane.showMessageDialog(null, "can't compile file : " + ce.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
		    }
		    catch(Exception er){
			JOptionPane.showMessageDialog(null, "can't do completion : " + er.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
		    }
		}
	    };
	compileMenu.getItem(2).addActionListener(co);
	comp.addActionListener(co);

	preferenceMenu.getItem(0).addActionListener(new ActionListener(){
		public void actionPerformed(ActionEvent e){
		    SettingFrame sf = new SettingFrame(settings);
		}
	    }
						    );

	

    }
    

    public void clean(JTable tab){
	for(int i = 0; i < tab.getRowCount(); i++){
	    for(int j = 0; j < tab.getColumnCount(); j++){
		if(j != 1)
		    tab.setValueAt("", i, j);
	    }
	}
    }
    
    public void clean(JTextField tf){
	tf.setText("");
    }
    
    public class KbcFileFilter extends javax.swing.filechooser.FileFilter{

	public KbcFileFilter() {
	}

	public boolean accept(File f) {
	    String path = f.getName();
	    if(path.endsWith(".kbc") || f.isDirectory())
		return true;
	    else 
		return false;
	}

	public String getDescription() {
	    return ".kbc files";
	}

    }

    private void loadSettings(){
	File set = new File("settings.completion");
	if(! set.exists())
	    settings = new Settings();
	else{
	    try{
		FileInputStream fis = new FileInputStream(set);
		ObjectInputStream ois = new ObjectInputStream(fis);
		settings = (Settings) ois.readObject();
	    }
	    catch(Exception er){
		JOptionPane.showMessageDialog(null, "load " + er.getMessage(), "Error !", JOptionPane.ERROR_MESSAGE);
	    }
	}
    }
}
