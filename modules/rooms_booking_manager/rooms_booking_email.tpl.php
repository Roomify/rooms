<h1>Booking request</h1>

<p>Name: <?php print $customer_name; ?></p>
<p>Email: <?php print $customer_email; ?></p>
<p>Address Line 1: <?php print $customer_add1; ?></p>
<p>Address Line 2: <?php print $customer_add2; ?></p>
<p>City: <?php print $customer_city; ?></p>
<p>State/County: <?php print $customer_state; ?></p>
<p>Country: <?php print $customer_country; ?></p>
<p>Comments: <?php print $comments; ?></p>

<p></p>

<h2>Enquiry</h2>
<?php
  foreach ($booking_request as $key => $value) {
  	$temp = explode(':', $key);
  	$label = db_select('rooms_unit_type', 'n')->fields('n', array('label'))->condition('type', $temp[0], '=')->execute()->fetchField();

  	print '<p>' . $label . ' (';
  	print $value[0] . ' - ' . $value[1] . ')</p>';
	}
?>