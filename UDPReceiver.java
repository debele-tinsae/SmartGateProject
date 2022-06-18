
import java.net.*;

public class UDPReceiver {

	private final static int PACKETSIZE = 100 ;
  protected DatagramPacket packet;
  

	public static void main( String args[] )
	{ 
		UDPReceiver rec = new UDPReceiver();
	      // Check the arguments
	      if( args.length != 1 )
	      {
	         System.out.println( "usage: UDPReceiver port" ) ;
	         return ;
	      }
	      try
	      {
	         // Convert the argument to ensure that is it valid
	         int port = Integer.parseInt( args[0] ) ;

	         // Construct the socket
	         DatagramSocket socket = new DatagramSocket( port ) ;
	         

	         for( ;; )
	         {
		        System.out.println( "Receiving on port " + port ) ;
		        
		       rec.packet = new DatagramPacket( new byte[PACKETSIZE], PACKETSIZE ) ;
	            socket.receive( rec.packet ) ;

	            System.out.println( rec.packet.getAddress() + " " + rec.packet.getPort() + ": " + new String(rec.packet.getData()).trim() ) ;
	        }  
	     }
	     catch( Exception e )
	     {
	        System.out.println( e ) ;
	     }
  }
	public String data() {
		return packet.getData().toString();
		
	}
}