 <?php 
/*
 * Copyright 2011 by Alex Lucyk, Jesus Naero, and Allen Tucker.
 * This program is part of RMH Homeroom, which is free software.
 * It comes with absolutely no warranty.  You can redistribute and/or
 * modify it under the terms of the GNU Public License as published
 * by the Free Software Foundation (see <http://www.gnu.org/licenses/).room.php
*/

// Get the array of rooms
$rooms = $roomLog->get_rooms();
$pendingandactivedayusecount = count(retrieve_pendingDayUse_dbBookings ($date)) + count(retrieve_active_day_use_dbBookings ($date));
?>

<!-- THIS SECTION DISPLAYS THE 14 ROOMS AND THEIR INFORMATION -->
<?php 
echo ("<table align=\"center\">");

// Use loops to construct each table row and column
for($i = 0; $i < 4; $i++){
	// Three rows
	echo ("<tr>");
	for($j = 0; $j < 4 && ($i*4)+$j < 14; $j++){
		// Retrieve the current room
		$currentRoomID = substr($rooms[($i*4)+$j],0,2);
		$currentBookingID = substr($rooms[($i*4)+$j],3);
		//echo ("current booking and room ids".$currentRoomID);
		//echo ($currentBookingID);
		$currentRoom = retrieve_dbRooms($currentRoomID,$date,$currentBookingID);
		$currentBooking = retrieve_dbBookings($currentBookingID);
		if ($date==date('y-m-d')){
            $roomStatus = $currentRoom->get_status();
            if ($currentBooking)
                $roomStatus="booked";
		}
        else if (!$currentBooking)
            $roomStatus = "clean";
        else $roomStatus = "booked";
		$statusmsg = $roomStatus;
		switch ($roomStatus) {
		    case "clean": $statusColor = "green"; $statusbg="White"; break;
		    case "reserved": $statusColor = "orange"; $statusbg="White"; break;
		    case "dirty": $statusColor = "red"; $statusmsg = "dirty"; $statusbg="Pink"; break;
		    case "booked": $statusColor = "brown"; $statusbg="White"; break;
		    case "off-line": $statusColor = "blue"; $statusbg="Gainsboro"; break;
		    default: $statusColor = null; $statusbg="White";
		}
		
		// This begins a "link" where the link is actually
		// the entire box. 
			echo ("<td id=\"nav\" onclick=\"window.location.href='room.php?room=".$currentRoomID."&date=".$date."&bookingID=".$currentBookingID."&status=".$roomStatus."'\"");
			echo ("style='vertical-align:text-top; background-color:".$statusbg."'>");

		// The ID of the room and the status
		// display "apt" instead of "08"
		if ($currentRoomID == "08") {
			echo ("<b style=\"float:left\">apt</b>");
		}
		else {
			echo ("<b style=\"float:left\">".$currentRoomID."</b>");
		}
		// Print the status of the room
		
		
		if($date == date("y-m-d"))  
		    if (!$currentBooking){
			    echo ("<b style=\"float:right; color:".$statusColor."\">");
			    echo ($statusmsg."</b>");
		    }
		    else if ($currentBooking->get_status()=="reserved")
		        echo ("<b style=\"float:right; color:orange\">reserved</b>");
		// Print the beds in the room
		echo("<br><b>Beds: </b>".$currentRoom->get_beds());
		// Print if there is a private bath
		if($currentRoom->get_bath() == "y"){
			echo ("<b> Bath</b>");
		}
		if($date == date("y-m-d") && $currentRoom->get_room_notes() != "")
		    echo "<br /><b>Notes: </b><br><span style='font-weight:bold;color:FF0000'>".$currentRoom->get_room_notes().'</span>';
		// Display info from the booking, such as the guests and loaners
		if (!$currentBooking)
		    echo "<br /><br /><br />";
		else 
			print_booking_info($currentBooking);
		// End this table item. Finish the link if it was started
		echo ("</td>");
		// see if there is a bath after this room
		$bathID = array_search($currentRoomID, array("B1|2"=>"01","B3|4"=>"03","B6|7"=>"06","B9|10"=>"09","B11|12"=>"11","B13|14"=>"13"));
		if ($bathID){ 
		    // pull its status from the dbRooms table and display it
		    $currentBath = retrieve_dbRooms($bathID,$date,$currentBookingID);
		    if ($date==date('y-m-d'))
		        $roomStatus = $currentBath->get_status();
		    else
		        $roomStatus = "clean";
		    $statusmsg = $roomStatus;
		    switch ($roomStatus) {
		            case "clean": $statusColor = "green"; $statusbg="White"; break;
		            case "dirty": $statusColor = "red"; $statusmsg = "dirty"; $statusbg="Pink"; break;
		            case "off-line": $statusColor = "blue"; $statusbg="Gainsboro"; break;
		            default: $statusColor = null; $statusbg="White";
		    }
		    echo ("<td id=\"nav\" onclick=\"window.location.href='room.php?room=".$bathID."&date=".$date.
		        "&bookingID=".$currentBookingID."&status=".$roomStatus."'\"");
		    echo ("style='vertical-align:text-top; background-color:".$statusbg."'>");
		    
		    // The ID of the room and the status
		    echo ("<b style=\"float:left\">".$bathID."</b>");
		    echo ("<b style=\"float:right; color:".$statusColor."\">");
		    if ($date==date('y-m-d'))
		        echo ($statusmsg);
		    echo "</b><br><b>Bath</b>";
		    
		    if ($currentBath->get_room_notes() != "")
		        echo "<br /><b>Notes: </b><br><span style='font-weight:bold;color:FF0000'>".$currentBath->get_room_notes().'</span>';
		    echo "<br /><br /><br />";
		    echo ("</td>");
		}
	}
	echo ("</tr>");
}

echo ("</tr></table>");


echo("<p>");

echo ("<b>Day Use Bookings: </b><br>");

$day_rooms = 20+$pendingandactivedayusecount;  // day use room #s begin at 21
$counter = 0;
echo ("<br><table align=\"center\">");
// Use loops to construct each table row and column
echo ("<tr>");
for($j = 21; $j < 21+$pendingandactivedayusecount; $j++)
		{
			$currentRoomID = substr($rooms[$counter+14],0,2);	
			$currentBookingID = substr($rooms[$counter+14],3);
			if ($currentRoomID!="" && $currentRoomID==$j) {
				$currentRoom = retrieve_dbRooms($currentRoomID,$date,$currentBookingID);
			    $currentBooking = retrieve_dbBookings($currentBookingID);
			}
			else {
				$currentRoomID = $j;
				insert_dbRooms(new Room($currentRoomID, null, null, null, "clean", null, ""));  //make sure there's a room there
				$currentBooking=null;
			}
			if ($currentBooking) {
				if ($date==date('y-m-d'))
            		$roomStatus = $currentRoom->get_status();
        		else if (!$currentBooking)
            		$roomStatus = "clean";
        		else $roomStatus = "booked";
				
        		// This begins a "link" where the link is actually
				// the entire box. 
				echo ("<td id=\"nav\" onclick=\"window.location.href='room.php?room=".$currentRoomID."&day=yes"."&date=".$date."&bookingID=".$currentBookingID."&status=".$roomStatus."'\"");
				echo ("style=\"vertical-align:text-top\">");
		
				// The ID of the room and the status
				// display "apt" instead of "08"
				if ($currentRoomID == "08") {
					echo ("<b style=\"float:left\">apt</b>");
				}
				else {
					echo ("<b style=\"float:left\">".$currentRoomID."</b>");
				}
				if($date == date("y-m-d") && $currentRoom->get_room_notes() != "")
				    echo "<br /><b>Notes: </b><br>".$currentRoom->get_room_notes();
				// Display info from the booking, such as the guests and loaners
			//	echo ("<br /><b>Guests:</b><br />");
				print_booking_info($currentBooking);
				echo ("</td>");
				$counter++;
			}
			else {
			    echo ("<td id=\"nav\" onclick=\"window.location.href='room.php?room=".$currentRoomID."&day=yes"."&date=".$date."&bookingID=&status=".$roomStatus."'\"");
				echo ("style=\"vertical-align:text-top\">");
				
				// display "apt" instead of "08"
				if ($currentRoomID == "08") {
					echo ("<b style=\"float:left\">apt</b>");
				}
				else {
					echo ("<b style=\"float:left\">".$currentRoomID."</b>");
				}
				echo "<br /><br /><br />";
				echo ("</td>");
			}	
		}	
echo ("</tr>");

echo ("</tr></table>");

/**
 * These functions help in printing out information
 * about each individual room or recieving information
 * needed to print
 */


// Function that gets the booking from a room
function get_booking($room){
	// Get the booking id
	$bookingID = $room->get_booking();
	// Retrieve the booking from the database
	$newBooking = retrieve_dbBookings($bookingID);
	// return the new booking
	return $newBooking;
}

// Funtion to display the information found in a booking.
function print_booking_info($booking){
	echo ("<br><b>Guests:</b>");
	$guests = $booking->get_occupants();
	// Print each occupant
	foreach($guests as $currentGuest) {
//	  if (strpos($currentGuest,"Present")>0) {
	  	$j = strpos($currentGuest,":");
	    if (!$j) $j = strlen($currentGuest);
		echo ("<br>".substr($currentGuest,0,$j));
//	  }
	}
	// Print the patient associated with the booking
	echo ("<br><b>Patient: </b><br>".$booking->getith_patient(0));
	for ($i=1; $i<3; $i++)
	    if ($booking->getith_patient($i))
	        echo ", ".$booking->getith_patient($i);
}
?>
