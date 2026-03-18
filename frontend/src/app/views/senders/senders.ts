import { ChangeDetectorRef, Component, inject, OnInit, signal } from '@angular/core';
import { FormsModule } from '@angular/forms';

import { AppModel } from '../../app.model';

type DeleteStatus = 'idle' | 'deleting' | 'success' | 'error';

@Component({
  selector: 'app-senders-view',
  imports: [FormsModule],
  templateUrl: './senders.html',
  styleUrl: './senders.scss',
})
export class SendersView implements OnInit {
  private cdr = inject(ChangeDetectorRef);

  model: AppModel  = { senderList: [] };
  email            = '';
  deleteStatus     = signal<DeleteStatus>('idle');

  ngOnInit(): void {
    if (typeof window.getSenders === 'function') {
      window.getSenders().then((senders) => {
        this.model.senderList = senders;
        this.cdr.detectChanges();
      });
    }
  }

  addSender(): void {
    const trimmed = this.email.trim();
    if (trimmed && !this.model.senderList.includes(trimmed)) {
      window.addToSenderList(trimmed).then(res => {
        this.model.senderList = res;
        this.cdr.detectChanges();
      })
      this.email = '';
    }
  }

  removeSender(index: number): void {
    const sender = this.model.senderList[index];
    window.removeFromSenderList(sender).then(res => {
      this.model.senderList = res;
      this.cdr.detectChanges();
    });
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
