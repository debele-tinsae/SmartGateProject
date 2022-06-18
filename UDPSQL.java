import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.mysql.cj.protocol.Resultset;

public class UDPSQL  {
	
	
	private String driver;
	private Connection connect = null; // to connect to the database
	private Statement stat = null;
	private Resultset res = null;
	private String url; // url for the database
	private UDPReceiver receive; 
	
	
	public UDPSQL(String driver,String url) {
		this.driver = driver;
		this.url = url;
	}
	
	
	public String url() {
		return this.url;
	}
	
	/*
	 Load the SQL driver
	 */
	public void LoadDriver(String driver) {
		try{
			Class.forName(driver);
		} catch (ClassNotFoundException e) {
			 
			  System.out.println("Could not find the database driver " + e.getMessage());
			   		
	}

  }
	public void ConnecttoDatabase(String username, String password) {
		try {
			connect = DriverManager.getConnection(this.url(),username,password);
		}catch(SQLException e) {
			System.out.println("Could not connect to database " + e.getMessage());
		}
	}
	/*
	 Get result of data in the table using row index and column index and compare with received data
	 */
	public int compareData(Statement stat, ResultSet res) { 
		try {
		stat = connect.createStatement();
		res = stat.executeQuery("SELECT plate FROM smartgate");
		
		// loop until end of the database
		while(res.next()) {
			
			String platenumber = res.getString("platenumber");
			
			//fetch rows
			System.out.print(res.getRow() +" "+platenumber);
			
			String data = platenumber;
			if (data == receive.data()) {
				return 1;
				
				
			}
			
		}
		
		
		
		
		
		
	} catch (SQLException e) {
		 
		  System.out.println("Could not retrieve data from the database " + e.getMessage());
		    }
		return 0;
	

	
}
	public void createNewDatabase(String driver,String url, String user,String pass) throws SQLException {
		  
		this.LoadDriver(driver);
        
       this.ConnecttoDatabase(user, pass);
       
       connect = DriverManager.getConnection(url,user,pass);
       
       stat = connect.createStatement();
       
       String sql = "CREATE DATABASE IF NOT EXISTS SMARTGATE";
       
       stat.executeUpdate(sql);
       
       System.out.println("Database being created");
       stat.close();
       connect.close();
       
      
    }
	
	public void createTable(String user,String pass)throws Exception {
		try {
		this.ConnecttoDatabase(user, pass);
		
		PreparedStatement create = connect.prepareStatement("CREATE TABLE IF NOT EXISTS smart(ID varchar(255), password varchar(255), platenumber varchar(255))");
	
		create.execute("USE SMARTGATE");
		create.executeUpdate();
		
	}catch(Exception e) {
		System.out.println(e);	
	}
	}
	
	public void addTable(String user,String pass) {
		final String plate1 = "ABCD";
		final int ID1 = 12;
		final String plate2 = "CDEN";
		final String plate3 = "";
		final String plate4 = "";
		final String plate5 = "";
	
		final int ID2= 80;
		final String password1 = "chicken";
		final String password2 = "football";
		
		try {
			this.ConnecttoDatabase(user, pass);
			
			//PreparedStatement add = connect.prepareStatement("INSERT INTO smart(ID,password, platenumber) VALUES ('"+ID1+"','"+password1+"','"+plate1+"')");
			//add.execute("USE SMARTGATE");
			//add.executeUpdate();
			PreparedStatement add1 = connect.prepareStatement("INSERT INTO smart(ID,password, platenumber) VALUES ('"+ID2+"','"+password2+"','"+plate2+"')");
			add1.execute("USE SMARTGATE");
			add1.executeUpdate();
			
		}catch(Exception e) {
			System.out.println(e);	
		}
		
	}
	
 
 
	public static void main( String args[] ) throws Exception
	{   
		String driver = "com.mysql.cj.jdbc.Driver";
		
		String url = "jdbc:mysql://localhost";
		UDPSQL sql = new UDPSQL(driver,url);
	    
	
		String user = "root";
		String pass = "yoha2000!";
		sql.createNewDatabase(driver,url,user,pass);
		sql.ConnecttoDatabase(user, pass);
		sql.createTable(user, pass);
		sql.addTable(user, pass);
		//Statement s = null;
		//ResultSet r = null;
		//sql.compareData(s, r);
		
		
	}
}
	
