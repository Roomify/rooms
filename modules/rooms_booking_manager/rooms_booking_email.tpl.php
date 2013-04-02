<h1><?php print t('Booking request'); ?></h1>

<p><?php print render($customer_name); ?></p>
<p><?php print render($customer_email); ?></p>
<p><?php print render($customer_add1); ?></p>
<p><?php print render($customer_add2); ?></p>
<p><?php print render($customer_city); ?></p>
<p><?php print render($customer_state); ?></p>
<p><?php print render($customer_country); ?></p>
<p><?php print render($comments); ?></p>

<p></p>

<h2><?php print t('Enquiry'); ?></h2>
<?php
  foreach ($booking_request as $request) {
    print render($request);
  }
?>
