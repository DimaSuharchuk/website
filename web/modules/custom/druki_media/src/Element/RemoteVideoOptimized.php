<?php

namespace Drupal\druki_media\Element;

use Drupal\Core\Render\Element\RenderElement;

/**
 * Provides optimized remote video media entity markup.
 *
 * @RenderElement("druki_media_remote_video_optimized")
 */
class RemoteVideoOptimized extends RenderElement {

  /**
   * {@inheritdoc}
   */
  public function getInfo() {
    return [
      '#theme' => 'druki_media_remote_video_optimized',
      '#attached' => [
        'library' => ['druki_media/remote_video_optimized'],
      ],
    ];
  }

}
