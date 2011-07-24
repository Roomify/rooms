<?php if (!$booking_results): ?>
  <?php print render($booking_search_form); ?>
  <?php print render($no_results); ?>
<?php endif; ?>

<?php if ($booking_results): ?>
  <?php foreach ($units as $unit_id => $unit) {
    print render($unit['unit']);
    print render($unit['price']);
    print render($unit['book_unit_form']);
  }?>
<?php endif; ?>
  
  
