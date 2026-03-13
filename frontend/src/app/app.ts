import './native-bridge';

import { Component, inject, OnInit, signal } from '@angular/core';

import { View } from './app.model';
import { ThemeService } from './services/theme.service';
import { Header } from './header/header';
import { Sidebar } from './sidebar/sidebar';
import { SendersView } from './views/senders/senders';
import { SettingsView } from './views/settings/settings';

@Component({
  selector: 'app-root',
  imports: [Header, Sidebar, SendersView, SettingsView],
  templateUrl: './app.html',
  styleUrl: './app.scss',
})
export class App implements OnInit {
  protected view = signal<View>('senders');

  private theme = inject(ThemeService);

  ngOnInit(): void {
    this.theme.init();
  }
}
