<?php

namespace Drupal\Tests\druki\Functional;

use Drupal\Tests\BrowserTestBase;

/**
 * Provides frontpage controller.
 *
 * @coversDefaultClass \Drupal\druki\Controller\FrontpageController
 */
final class FrontpageControllerTest extends BrowserTestBase {

  /**
   * {@inheritdoc}
   */
  public static $modules = ['druki'];

  /**
   * Test page response.
   *
   * @covers ::build
   */
  public function testPage() {
    $this->drupalGet('/frontpage');
    // Since this page just for URI and actual markup added via theme we check
    // it exists and response as expected, thats it.
    $this->assertResponse(200);
  }

}
