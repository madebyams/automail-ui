import { ChangeDetectorRef, Component, inject, signal } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { AppModel } from '../../app.model';

type DeleteStatus = 'idle' | 'deleting' | 'success' | 'error';

@Component({
  selector: 'app-senders-view',
  imports: [FormsModule],
  templateUrl: './senders.html',
  styleUrl: './senders.scss',
})
export class SendersView {
  private cdr = inject(ChangeDetectorRef);

  model: AppModel  = { senderList: [] };
  email            = '';
  deleteStatus     = signal<DeleteStatus>('idle');

  addSender(): void {
    const trimmed = this.email.trim();
    if (trimmed && !this.model.senderList.includes(trimmed)) {
      this.model.senderList.push(trimmed);
      this.email = '';
    }
  }

  removeSender(index: number): void {
    this.model.senderList.splice(index, 1);
  }

  async processAndDelete(): Promise<void> {
    this.deleteStatus.set('deleting');
    try {
      await window.doDelete(this.model);
      this.model.senderList = [];
      this.deleteStatus.set('success');
    } catch {
      this.deleteStatus.set('error');
    } finally {
      this.cdr.markForCheck();
      setTimeout(() => {
        this.deleteStatus.set('idle');
        this.cdr.markForCheck();
      }, 4000);
    }
  }

  dismissBanner(): void {
    this.deleteStatus.set('idle');
  }
}
