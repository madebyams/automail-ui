import { ChangeDetectorRef, Component, OnInit, inject, signal } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { ClientType, Config, SaveStatus, defaultConfig } from '../../app.model';

@Component({
  selector: 'app-settings-view',
  imports: [FormsModule],
  templateUrl: './settings.html',
  styleUrl: './settings.scss',
})
export class SettingsView implements OnInit {
  private cdr = inject(ChangeDetectorRef);

  readonly ClientType = ClientType;

  imap: Config = defaultConfig();
  showPassword        = signal(false);
  saveStatus          = signal<SaveStatus>('idle');

  isImap(): boolean { return this.imap.client_type === ClientType.IMAP; }

  ngOnInit(): void {
    if (typeof window.loadImapConfig === 'function') {
      window.loadImapConfig().then((cfg) => {
        if (cfg) {
          this.imap = cfg;
          this.cdr.markForCheck();
        }
      });
    }
  }

  async saveImap(): Promise<void> {
    this.saveStatus.set('saving');
    try {
      if (typeof window.saveImapConfig === 'function') {
        await window.saveImapConfig(this.imap);
      }
      this.saveStatus.set('saved');
    } catch {
      this.saveStatus.set('error');
    } finally {
      setTimeout(() => this.saveStatus.set('idle'), 2500);
    }
  }

  onSslToggle(): void {
    if (this.imap.ssl  && this.imap.port === 143) this.imap.port = 993;
    if (!this.imap.ssl && this.imap.port === 993) this.imap.port = 143;
  }
}
