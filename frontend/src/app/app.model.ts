export type View       = 'senders' | 'settings';
export type SaveStatus = 'idle' | 'saving' | 'saved' | 'error';

export enum ClientType {
    IMAP = 'imap',
    COM  = 'com',
}

export interface AppModel {
    senderList: string[];
}

export interface Config {
    hostname:     string;
    port:         number;
    username:     string;
    password:     string;
    ssl:          boolean;
    mailbox:      string;
    client_type:  ClientType;
}

export const defaultConfig = (): Config => ({
    hostname:    '',
    port:        993,
    username:    '',
    password:    '',
    ssl:         true,
    mailbox:     'INBOX',
    client_type: ClientType.IMAP,
});