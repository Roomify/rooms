<?php

/**
 * @file
 * Example tpl file for theming a single room-specific theme
 *
 * Available variables:
 * - $status: The variable to theme (while only show if you tick status)
 * 
 * Helper variables:
 * - $rooms_room: The Room object the variables are derived from
 */
?>

<div class="rooms_room-status">
  <?php print '<strong>Rooms Sample Data:</strong> ' . $rooms_room_sample_data = ($rooms_room_sample_data) ? 'Switch On' : 'Switch Off' ?>
</div>