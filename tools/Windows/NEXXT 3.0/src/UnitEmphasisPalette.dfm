object FormEmphasisPalette: TFormEmphasisPalette
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = 'Generate emphasis (1536b) palette'
  ClientHeight = 544
  ClientWidth = 413
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBox1: TPaintBox
    Left = 395
    Top = 520
    Width = 16
    Height = 22
    OnPaint = PaintBox1Paint
  end
  object ImageEmphPalette: TImage
    Left = 154
    Top = 8
    Width = 256
    Height = 512
    OnClick = ImageEmphPaletteClick
    OnMouseMove = ImageEmphPaletteMouseMove
  end
  object SpeedButton17: TSpeedButton
    Left = 6
    Top = 34
    Width = 57
    Height = 17
    Caption = 'Save 192b'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton18: TSpeedButton
    Left = 4
    Top = 518
    Width = 63
    Height = 23
    Caption = 'Save 1536b'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton18Click
  end
  object SpeedButton19: TSpeedButton
    Left = 73
    Top = 518
    Width = 74
    Height = 23
    Caption = 'Save bmp'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton19Click
  end
  object SpeedButton20: TSpeedButton
    Left = 288
    Top = 524
    Width = 44
    Height = 17
    Caption = 'copy hex'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton20Click
  end
  object SpeedButton21: TSpeedButton
    Left = 154
    Top = 524
    Width = 78
    Height = 17
    Caption = 'reset default bits'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton21Click
  end
  object SpeedButton30: TSpeedButton
    Left = 238
    Top = 524
    Width = 44
    Height = 17
    Caption = 'method'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    OnClick = SpeedButton30Click
  end
  object GroupBox1: TGroupBox
    Left = 4
    Top = 3
    Width = 144
    Height = 59
    Caption = 'File address 0x000'
    TabOrder = 0
    object SpeedButton1: TSpeedButton
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton2: TSpeedButton
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton22: TSpeedButton
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB0: TCheckBox
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B0'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG0: TCheckBox
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G0'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR0: TCheckBox
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R0'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object GroupBox2: TGroupBox
    Left = 4
    Top = 67
    Width = 144
    Height = 59
    Caption = 'File address 0x0c0'
    TabOrder = 1
    object SpeedButton3: TSpeedButton
      Tag = 1
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton4: TSpeedButton
      Tag = 1
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton23: TSpeedButton
      Tag = 1
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB1: TCheckBox
      Tag = 1
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B1'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG1: TCheckBox
      Tag = 1
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G1'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR1: TCheckBox
      Tag = 1
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R1'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object GroupBox3: TGroupBox
    Left = 4
    Top = 131
    Width = 144
    Height = 59
    Caption = 'File address 0x180'
    TabOrder = 2
    object SpeedButton5: TSpeedButton
      Tag = 2
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton6: TSpeedButton
      Tag = 2
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton24: TSpeedButton
      Tag = 2
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB2: TCheckBox
      Tag = 2
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B2'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG2: TCheckBox
      Tag = 2
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G2'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR2: TCheckBox
      Tag = 2
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R2'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object GroupBox4: TGroupBox
    Left = 4
    Top = 195
    Width = 144
    Height = 59
    Caption = 'File address 0x240'
    TabOrder = 3
    object SpeedButton7: TSpeedButton
      Tag = 3
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton8: TSpeedButton
      Tag = 3
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton25: TSpeedButton
      Tag = 3
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB3: TCheckBox
      Tag = 3
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B3'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG3: TCheckBox
      Tag = 3
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G3'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR3: TCheckBox
      Tag = 3
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R3'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object GroupBox5: TGroupBox
    Left = 4
    Top = 259
    Width = 144
    Height = 59
    Caption = 'File address 0x300'
    TabOrder = 4
    object SpeedButton9: TSpeedButton
      Tag = 4
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton10: TSpeedButton
      Tag = 4
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton26: TSpeedButton
      Tag = 4
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB4: TCheckBox
      Tag = 4
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B4'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG4: TCheckBox
      Tag = 4
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G4'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR4: TCheckBox
      Tag = 4
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R4'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object GroupBox6: TGroupBox
    Left = 4
    Top = 323
    Width = 144
    Height = 59
    Caption = 'File address 0x3c0'
    TabOrder = 5
    object SpeedButton11: TSpeedButton
      Tag = 5
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton12: TSpeedButton
      Tag = 5
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton27: TSpeedButton
      Tag = 5
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB5: TCheckBox
      Tag = 5
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B5'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG5: TCheckBox
      Tag = 5
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G5'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR5: TCheckBox
      Tag = 5
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R5'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object GroupBox7: TGroupBox
    Left = 4
    Top = 387
    Width = 144
    Height = 59
    Caption = 'File address 0x480'
    TabOrder = 6
    object SpeedButton13: TSpeedButton
      Tag = 6
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton14: TSpeedButton
      Tag = 6
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton28: TSpeedButton
      Tag = 6
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB6: TCheckBox
      Tag = 6
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B6'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG6: TCheckBox
      Tag = 6
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G6'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR6: TCheckBox
      Tag = 6
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R6'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object GroupBox8: TGroupBox
    Left = 4
    Top = 451
    Width = 144
    Height = 59
    Caption = 'File address 0x540'
    TabOrder = 7
    object SpeedButton15: TSpeedButton
      Tag = 7
      Left = 6
      Top = 34
      Width = 57
      Height = 17
      Caption = 'Save 192b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton1Click
    end
    object SpeedButton16: TSpeedButton
      Tag = 7
      Left = 69
      Top = 34
      Width = 67
      Height = 17
      Caption = 'Save bmp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton2Click
    end
    object SpeedButton29: TSpeedButton
      Tag = 7
      Left = 6
      Top = 18
      Width = 17
      Height = 13
      Caption = '...'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = SpeedButton22Click
    end
    object chkB7: TCheckBox
      Tag = 7
      Left = 33
      Top = 16
      Width = 30
      Height = 17
      Caption = 'B7'
      TabOrder = 0
      OnMouseDown = chkB0MouseDown
    end
    object chkG7: TCheckBox
      Tag = 7
      Left = 69
      Top = 16
      Width = 30
      Height = 17
      Caption = 'G7'
      TabOrder = 1
      OnMouseDown = chkG0MouseDown
    end
    object chkR7: TCheckBox
      Tag = 7
      Left = 105
      Top = 16
      Width = 30
      Height = 17
      Caption = 'R7'
      TabOrder = 2
      OnMouseDown = chkR0MouseDown
    end
  end
  object MaskEdit1: TMaskEdit
    Left = 332
    Top = 524
    Width = 62
    Height = 19
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -9
    Font.Name = 'Tahoma'
    Font.Style = []
    MaxLength = 6
    ParentFont = False
    TabOrder = 8
    Text = 'click a swatch'
  end
  object SaveDialogPal: TSaveDialog
    Left = 163
    Top = 19
  end
  object SaveDialogBitmap: TSaveDialog
    DefaultExt = 'bmp'
    Filter = 'Windows bitmap (*.bmp)|*.bmp|All files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 194
    Top = 19
  end
  object SaveDialogFullPal: TSaveDialog
    Left = 162
    Top = 54
  end
  object SaveDialogFullBitmap: TSaveDialog
    DefaultExt = 'bmp'
    Filter = 'Windows bitmap (*.bmp)|*.bmp|All files (*.*)|*.*'
    Left = 195
    Top = 54
  end
  object PopupMenu1: TPopupMenu
    Left = 163
    Top = 94
    object PutCarrayonclipboard1: TMenuItem
      Caption = 'to clipboard as C / C++'
      OnClick = PutCarrayonclipboard1Click
    end
    object toclipboardasjavac1: TMenuItem
      Tag = 1
      Caption = 'to clipboard as Java / C#'
      OnClick = PutCarrayonclipboard1Click
    end
  end
  object PopupMenuMethod: TPopupMenu
    Left = 197
    Top = 96
    object NESPPU1: TMenuItem
      Caption = 'NES PPU (Chris Covell method)'
      Checked = True
      Default = True
      GroupIndex = 1
      RadioItem = True
      OnClick = NESPPU1Click
    end
    object emulatorspecific1: TMenuItem
      Caption = 'Emulator specific methods'
      GroupIndex = 1
      object Nintendulator1: TMenuItem
        Caption = 'Nintendulator v0.98'
        GroupIndex = 1
        RadioItem = True
        OnClick = Nintendulator1Click
      end
      object Mesen1: TMenuItem
        Caption = 'Mesen 2'
        GroupIndex = 1
        RadioItem = True
        OnClick = Mesen1Click
      end
    end
    object N1: TMenuItem
      Caption = '-'
      GroupIndex = 1
    end
    object RGBPPUPlaychoiceetc1: TMenuItem
      Caption = 'RGB PPU (Playchoice and Vs. System)'
      GroupIndex = 1
      RadioItem = True
      OnClick = RGBPPUPlaychoiceetc1Click
    end
  end
end
