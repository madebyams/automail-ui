import { Component, inject } from '@angular/core';

import { ThemeService } from '../services/theme.service';

@Component({
  selector: 'app-header',
  templateUrl: './header.html',
  styleUrl: './header.scss',
})
export class Header {
  protected theme = inject(ThemeService);
}
