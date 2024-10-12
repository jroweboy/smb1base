object FormNewInstanceMinimal: TFormNewInstanceMinimal
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'New instance...'
  ClientHeight = 129
  ClientWidth = 267
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnOK: TButton
    Left = 203
    Top = 100
    Width = 61
    Height = 25
    Caption = 'Create'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object GroupBox1: TGroupBox
    Left = 132
    Top = 3
    Width = 133
    Height = 94
    Caption = 'New session map size'
    TabOrder = 1
    object BtnWdtInc: TSpeedButton
      Left = 8
      Top = 16
      Width = 33
      Height = 14
      Caption = 'w+32'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnWdtIncClick
    end
    object BtnWdtDec: TSpeedButton
      Left = 8
      Top = 54
      Width = 33
      Height = 14
      Caption = 'w -32'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnWdtDecClick
    end
    object BtnHgtInc: TSpeedButton
      Left = 72
      Top = 16
      Width = 33
      Height = 14
      Caption = 'h+30'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnHgtIncClick
    end
    object BtnHgtDec: TSpeedButton
      Left = 72
      Top = 54
      Width = 33
      Height = 14
      Caption = 'h -30'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnHgtDecClick
    end
    object Btn32x30: TSpeedButton
      Left = 72
      Top = 72
      Width = 40
      Height = 16
      Caption = '32x30'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = Btn32x30Click
    end
    object BtnThisSession: TSpeedButton
      Left = 8
      Top = 72
      Width = 62
      Height = 16
      Caption = '= this session'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -9
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = BtnThisSessionClick
    end
    object EditWidth: TEdit
      Left = 8
      Top = 31
      Width = 33
      Height = 21
      TabOrder = 0
      Text = '32'
    end
    object UpDownWidth: TUpDown
      Left = 41
      Top = 31
      Width = 16
      Height = 21
      Associate = EditWidth
      Min = 4
      Max = 4096
      Increment = 4
      Position = 32
      TabOrder = 1
      Thousands = False
    end
    object EditHeight: TEdit
      Left = 72
      Top = 31
      Width = 33
      Height = 21
      TabOrder = 2
      Text = '30'
    end
    object UpDownHeight: TUpDown
      Left = 105
      Top = 31
      Width = 16
      Height = 21
      Associate = EditHeight
      Min = 4
      Max = 4096
      Increment = 4
      Position = 30
      TabOrder = 3
      Thousands = False
    end
  end
  object GroupBox2: TGroupBox
    Left = 5
    Top = 3
    Width = 124
    Height = 54
    Caption = 'Choose...'
    TabOrder = 2
    object RadioNew: TRadioButton
      Left = 5
      Top = 16
      Width = 113
      Height = 17
      Caption = 'New session'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = RadioNewClick
    end
    object RadioClone: TRadioButton
      Left = 5
      Top = 32
      Width = 113
      Height = 17
      Caption = 'Clone this session'
      TabOrder = 1
      OnClick = RadioCloneClick
    end
  end
  object Button1: TButton
    Left = 132
    Top = 100
    Width = 61
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object GroupBoxIfNew: TGroupBox
    Left = 5
    Top = 56
    Width = 124
    Height = 70
    Caption = 'New session inherits'
    TabOrder = 4
    object chkInheritSubpal: TCheckBox
      Left = 5
      Top = 16
      Width = 110
      Height = 17
      Caption = 'Inherit subpalettes'
      TabOrder = 0
    end
    object chkInheritCHR: TCheckBox
      Left = 5
      Top = 32
      Width = 97
      Height = 17
      Caption = 'Inherit CHR'
      TabOrder = 1
    end
    object chkInheritMap: TCheckBox
      Left = 5
      Top = 48
      Width = 97
      Height = 17
      Caption = 'Inherit map'
      TabOrder = 2
      OnClick = chkInheritMapClick
    end
  end
end
