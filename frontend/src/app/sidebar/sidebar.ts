import { Component, input, output } from '@angular/core';

import { View } from '../app.model';

@Component({
  selector: 'app-sidebar',
  templateUrl: './sidebar.html',
  styleUrl: './sidebar.scss',
})
export class Sidebar {
  activeView = input.required<View>();
  viewChange  = output<View>();
}
