import java.net.*;
import java.util.Scanner;
import java.util.Random;

public class UDPSender {

	public static void main(String[] args) {
		// Check the arguments
		if (args.length != 3) {
			System.out.println("usage: java UDPSender host port");
			return;
		}
		DatagramSocket socket = null;
		try {
			// Convert the arguments first, to ensure that they are valid
			InetAddress host = InetAddress.getByName(args[0]);
			int port = Integer.parseInt(args[1]);
			Integer messageNumber = Integer.parseInt(args[2]);
			socket = new DatagramSocket();

			// Scanner in;
			// in = new Scanner (System.in);

			DatagramSocket socket2 = new DatagramSocket(1002);
			Random random = new Random();
			Integer rnum;

			while (true) {

				System.out.println("ENTER to quit ");
				for (int i = 0; i < messageNumber; i++) {
					rnum = random.nextInt();
					String num = rnum.toString();
					System.out.println(num);
					if (messageNumber == 0)
						break;
					byte[] data = num.getBytes();
					DatagramPacket packet = new DatagramPacket(data, data.length, host, port);
					socket.send(packet);
					// qqSystem.out.println("Waiting for ACK");
					// socket2.receive( packet ) ;

				}
				break;
			}
			System.out.println("Closing down");
		} catch (Exception e) {
			System.out.println(e);
		} finally {
			if (socket != null)
				socket.close();
		}
	}
}



