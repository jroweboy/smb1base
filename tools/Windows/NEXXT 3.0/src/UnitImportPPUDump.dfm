object FormImportPPUDump: TFormImportPPUDump
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Import PPU dump'
  ClientHeight = 193
  ClientWidth = 310
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 0
    Width = 297
    Height = 41
    Caption = 'Import sources'
    TabOrder = 0
    object CheckCHR: TCheckBox
      Left = 16
      Top = 16
      Width = 60
      Height = 17
      Caption = 'CHR'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object CheckName: TCheckBox
      Left = 80
      Top = 16
      Width = 86
      Height = 17
      Caption = 'Nametables'
      Checked = True
      State = cbChecked
      TabOrder = 1
    end
    object CheckSubpal: TCheckBox
      Left = 180
      Top = 16
      Width = 97
      Height = 17
      Caption = 'Subpalettes'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 42
    Width = 297
    Height = 73
    Caption = 'Nametables source'
    TabOrder = 1
    object RadioAll: TRadioButton
      Left = 11
      Top = 18
      Width = 49
      Height = 17
      Caption = 'All 4'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object RadioHorz: TRadioButton
      Left = 66
      Top = 18
      Width = 113
      Height = 17
      Caption = '2 (Horizontal arr.)'
      TabOrder = 1
    end
    object RadioVert: TRadioButton
      Left = 185
      Top = 18
      Width = 90
      Height = 17
      Caption = '2 (Vertical arr.)'
      TabOrder = 2
    end
    object Radio2000: TRadioButton
      Left = 11
      Top = 34
      Width = 89
      Height = 17
      Caption = '1 (@ $2000)'
      TabOrder = 3
    end
    object Radio2400: TRadioButton
      Left = 104
      Top = 34
      Width = 84
      Height = 17
      Caption = '1 (@ $2400)'
      TabOrder = 4
    end
    object Radio2800: TRadioButton
      Left = 11
      Top = 50
      Width = 84
      Height = 17
      Caption = '1 (@ $2800)'
      TabOrder = 5
    end
    object Radio2C00: TRadioButton
      Left = 104
      Top = 50
      Width = 89
      Height = 17
      Caption = '1 (@ $2C00)'
      TabOrder = 6
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 115
    Width = 116
    Height = 74
    Caption = 'CHR source'
    TabOrder = 2
    object Radio8k: TRadioButton
      Left = 11
      Top = 18
      Width = 100
      Height = 17
      Caption = '8k (Both tables)'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object Radio0000: TRadioButton
      Left = 11
      Top = 34
      Width = 84
      Height = 17
      Caption = '4k (@ $0000)'
      TabOrder = 1
    end
    object Radio1000: TRadioButton
      Left = 11
      Top = 50
      Width = 84
      Height = 17
      Caption = '4k (@ $1000)'
      TabOrder = 2
    end
  end
  object GroupBox4: TGroupBox
    Left = 120
    Top = 115
    Width = 106
    Height = 74
    Caption = 'Subpalettes source'
    TabOrder = 3
    object RadioPalBoth: TRadioButton
      Left = 11
      Top = 18
      Width = 80
      Height = 17
      Caption = 'Both sets'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object RadioPalBG: TRadioButton
      Left = 11
      Top = 34
      Width = 84
      Height = 17
      Caption = 'BG set only'
      TabOrder = 1
    end
    object RadioPalSPR: TRadioButton
      Left = 11
      Top = 50
      Width = 84
      Height = 17
      Caption = 'SPR set only'
      TabOrder = 2
    end
  end
  object Button1: TButton
    Left = 232
    Top = 162
    Width = 70
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 232
    Top = 131
    Width = 70
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 5
    OnClick = Button2Click
  end
end
