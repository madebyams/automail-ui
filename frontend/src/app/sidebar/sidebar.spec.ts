import { Component } from '@angular/core';
import { TestBed } from '@angular/core/testing';
import { Sidebar } from './sidebar';
import { View } from '../app.model';

@Component({
  standalone: true,
  imports: [Sidebar],
  template: '<app-sidebar [activeView]="view" (viewChange)="emitted = $event" />',
})
class TestHost {
  view: View = 'senders';
  emitted: View | null = null;
}

describe('Sidebar', () => {
  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [TestHost],
    }).compileComponents();
  });

  it('should create inside a host', () => {
    const fixture = TestBed.createComponent(TestHost);
    fixture.detectChanges();
    expect(fixture.componentInstance).toBeTruthy();
  });

  it('accepts activeView input', () => {
    const fixture = TestBed.createComponent(TestHost);
    fixture.componentInstance.view = 'settings';
    fixture.detectChanges();
    const sidebar = fixture.debugElement.children[0].componentInstance as Sidebar;
    expect(sidebar.activeView()).toBe('settings');
  });
});
