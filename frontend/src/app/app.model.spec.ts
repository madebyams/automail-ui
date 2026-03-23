import { ClientType, defaultConfig } from './app.model';

describe('defaultConfig', () => {
  it('returns expected defaults', () => {
    const config = defaultConfig();
    expect(config.hostname).toBe('');
    expect(config.port).toBe(993);
    expect(config.username).toBe('');
    expect(config.password).toBe('');
    expect(config.ssl).toBe(true);
    expect(config.mailbox).toBe('INBOX');
    expect(config.client_type).toBe(ClientType.IMAP);
  });

  it('returns a new object on each call', () => {
    expect(defaultConfig()).not.toBe(defaultConfig());
  });
});
