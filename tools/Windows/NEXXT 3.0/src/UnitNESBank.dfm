object FormBank: TFormBank
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'FormBank'
  ClientHeight = 340
  ClientWidth = 542
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object GroupBoxInfo: TGroupBox
    Left = 0
    Top = 0
    Width = 542
    Height = 340
    Align = alClient
    TabOrder = 0
    object ImageCHR1: TImage
      Left = 8
      Top = 38
      Width = 256
      Height = 256
    end
    object ImageCHR2: TImage
      Left = 278
      Top = 38
      Width = 256
      Height = 256
    end
    object LabelInfo: TLabel
      Left = 8
      Top = 15
      Width = 52
      Height = 16
      Caption = 'LabelInfo'
    end
    object Label1: TLabel
      Left = 8
      Top = 311
      Width = 32
      Height = 16
      Caption = 'Bank:'
    end
    object Label2: TLabel
      Left = 278
      Top = 16
      Width = 75
      Height = 16
      Caption = 'LabelFromTo'
    end
    object btnDiffCheck: TSpeedButton
      Left = 430
      Top = 12
      Width = 60
      Height = 22
      AllowAllUp = True
      GroupIndex = 3
      Caption = 'Diff check'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnDiffCheckClick
    end
    object btnInverse: TSpeedButton
      Left = 492
      Top = 12
      Width = 26
      Height = 22
      AllowAllUp = True
      GroupIndex = 4
      Caption = '(inv)'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      OnClick = btnDiffCheckClick
    end
    object ButtonOK: TButton
      Left = 407
      Top = 308
      Width = 61
      Height = 25
      Caption = 'OK'
      Enabled = False
      TabOrder = 0
      OnClick = ButtonOKClick
    end
    object ButtonCancel: TButton
      Left = 474
      Top = 308
      Width = 60
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = ButtonCancelClick
    end
    object EditBank: TEdit
      Left = 52
      Top = 308
      Width = 41
      Height = 24
      TabOrder = 2
      Text = '0'
      OnChange = EditBankChange
    end
    object UpDownBank: TUpDown
      Left = 93
      Top = 308
      Width = 19
      Height = 24
      Associate = EditBank
      TabOrder = 3
    end
  end
  object GroupBox1: TGroupBox
    Left = 278
    Top = 294
    Width = 122
    Height = 41
    Caption = 'Set'
    TabOrder = 1
    object btnA: TSpeedButton
      Left = 3
      Top = 16
      Width = 28
      Height = 22
      GroupIndex = 1
      Caption = 'A'
      OnClick = btnAClick
    end
    object btnB: TSpeedButton
      Tag = 1
      Left = 32
      Top = 16
      Width = 28
      Height = 22
      GroupIndex = 1
      Caption = 'B'
      OnClick = btnAClick
    end
    object btnC: TSpeedButton
      Tag = 2
      Left = 61
      Top = 16
      Width = 28
      Height = 22
      GroupIndex = 1
      Caption = 'C'
      OnClick = btnAClick
    end
    object btnD: TSpeedButton
      Tag = 3
      Left = 90
      Top = 16
      Width = 28
      Height = 22
      GroupIndex = 1
      Caption = 'D'
      OnClick = btnAClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 120
    Top = 294
    Width = 146
    Height = 41
    Caption = 'Preview with palette'
    TabOrder = 2
    object btnGray: TSpeedButton
      Left = 3
      Top = 16
      Width = 32
      Height = 22
      GroupIndex = 2
      Caption = 'Gray'
      OnClick = btnGrayClick
    end
    object btnPal0: TSpeedButton
      Left = 36
      Top = 16
      Width = 26
      Height = 22
      GroupIndex = 2
      Caption = '0'
      OnClick = btnAClick
    end
    object btnPal1: TSpeedButton
      Left = 63
      Top = 16
      Width = 26
      Height = 22
      GroupIndex = 2
      Caption = '1'
      OnClick = btnAClick
    end
    object btnPal2: TSpeedButton
      Left = 90
      Top = 16
      Width = 26
      Height = 22
      GroupIndex = 2
      Caption = '2'
      OnClick = btnAClick
    end
    object btnPal3: TSpeedButton
      Left = 117
      Top = 16
      Width = 25
      Height = 22
      GroupIndex = 2
      Caption = '3'
      OnClick = btnAClick
    end
  end
  object TrackBar1: TTrackBar
    Left = 522
    Top = 8
    Width = 18
    Height = 30
    LineSize = 20
    Max = 255
    Min = 20
    Orientation = trVertical
    PageSize = 10
    Frequency = 20
    Position = 120
    TabOrder = 3
    ThumbLength = 14
    TickStyle = tsNone
    OnChange = TrackBar1Change
  end
end
