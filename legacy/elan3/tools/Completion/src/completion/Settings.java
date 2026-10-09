package completion;

import java.io.*;


/* This class contains paths for elanc and gmake, which are *
 * used to compile the source specification. The user has to*
 * set them once. Then the paths will be written in the file*
 * settings.completion and reloaded at each launching of the*
 * program. */

public class Settings implements Serializable{


    /* The paths */
    private String gmakePath = null;
    private String elanPath = null;
    
    /* The default constructor. It is used when the user has *
     * correctly set his PATH variable. */
    public Settings(){
	this("gmake", "elanc");
    }

    /* The general constructor */
    public Settings(String gmake, String elan){
	gmakePath = gmake;
	elanPath = elan;
    }

    /* This method save the current object into the file     *
     * settings.completion. */
    public void save() throws Exception{
	File set = new File("settings.completion");
	FileOutputStream fos = new FileOutputStream(set);
	ObjectOutputStream oos = new ObjectOutputStream(fos);
	oos.writeObject(this);
	oos.close();
	fos.close();
    }

    /* Methods modifying fields*/
    public void setElan(String elan){
	elanPath = elan;
    }

    public void setGmake(String gmake){
	gmakePath = gmake;
    }

    /* Methods to access fields */
    public String getElan(){
	return elanPath;
    }

    public String getGmake(){
	return gmakePath;
    }
}
